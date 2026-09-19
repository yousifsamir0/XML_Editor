#include "tree.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
#include "constants.h"
#include <fstream>

// Prettify the subtree rooted at `current_node` into an indented XML string.
// `tab` contains the current indentation prefix.
auto Tree::prettify(Node *current_node, string tab) -> string {

  string s;
  bool flag = false;
  bool is_header = current_node->tag_name.substr(0, 4) == "?xml";
  if (current_node->getIsComment() && !is_header) {
    s += tab + xml_editor::constants::LT + current_node->tag_name +
         xml_editor::constants::GT;
  } else if (current_node->getIsSelfClose() && !is_header) {
    s += tab + xml_editor::constants::LT + current_node->tag_name +
         xml_editor::constants::GT;
  }

  else if (current_node->is_tag && !is_header) {
    s += tab + xml_editor::constants::LT + current_node->tag_name + " ";
    s += current_node->attr;
    s += xml_editor::constants::GT;
    flag = true;
  } else if (!is_header) {
    if (current_node->children.empty()) {
      s += (current_node->tag_name);
    } else {
      s += "\n";
      s += (current_node->tag_name);
    }
  }

  if (current_node->is_tag && !current_node->getHaveData() && !is_header) {
    s += xml_editor::constants::NEWLINE;
  }

  for (auto &i : current_node->children) {
    if (is_header || i->getIsComment()) {
      s += prettify(i, tab);
    } else {
      s += prettify(i, "      " + tab);
    }
  }

  if (flag && current_node->getHaveData() && current_node->is_valid) {

    s += std::string(xml_editor::constants::LT_SLASH) + current_node->tag_name +
         xml_editor::constants::GT + xml_editor::constants::NEWLINE;
    flag = false;
  } else if (flag && current_node->is_valid) {
    s += tab + std::string(xml_editor::constants::LT_SLASH) +
         current_node->tag_name + xml_editor::constants::GT +
         xml_editor::constants::NEWLINE;
  }

  else if (flag && current_node->getHaveData() &&
           current_node->children.empty()) {
    s += std::string(xml_editor::constants::LT_SLASH) +
         current_node->getCorrectTag() + xml_editor::constants::GT +
         xml_editor::constants::NEWLINE;
    flag = false;
  } else if (flag) {
    s += tab + std::string(xml_editor::constants::LT_SLASH) +
         current_node->tag_name + xml_editor::constants::GT +
         xml_editor::constants::NEWLINE;
    flag = false;
  }

  return s;
}

// Convenience wrapper: convert entire tree to JSON starting from root.
auto Tree::convert_json() -> string {
  return this->convert_json(this->getRoot(), "");
}

auto Tree::write_json_to_file(const std::string &filepath) -> bool {
  std::ofstream ofs(filepath);
  if (!ofs.is_open()) {
    return false;
  }
  ofs << this->convert_json();
  ofs.close();
  return true;
}

// Convert a subtree to a JSON-like representation.
// This function walks nodes and emits JSON with simple heuristics
// (attributes prefixed with '_' and repeated tags emitted as arrays).
auto Tree::convert_json(Node *current_node, string tab) -> string {
  string s;
  bool is_root_node = (current_node == this->getRoot());
  if (current_node->getIsComment()) {
    return s;
  }
  if (current_node->getIsSelfClose()) {
    int i = 0;
    s += tab + "\"";
    while (current_node->tag_name[i] != ' ') {
      s += current_node->tag_name[i];
      i++;
    }
    i++;
    s += "\":{";
    while (current_node->tag_name[i] != '/') {
      s += "\"_";

      while (current_node->tag_name[i] != '=') {

        s += current_node->tag_name[i];
        i++;
      }
      i++;
      s += "\":";
      while (current_node->tag_name[i] != ' ' &&
             current_node->tag_name[i] != '/') {
        s += current_node->tag_name[i];
        i++;
      }
      if (current_node->tag_name[i] == ' ') {
        i++;
      }
    }
    s += "}";

    return s;
  }
  if (current_node->xml) {
    for (auto i : current_node->children) {
      if (i->tag_name.substr(0, 4) != "?xml") {
        current_node = i;
        is_root_node = true;
      }
    }
  }
  if (current_node->is_tag && !current_node->getHaveData()) {
    vector<string> v;
    for (auto *i : current_node->children) {
      v.push_back(i->tag_name);
    }
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    if (is_root_node) {
      s += "{\n";
      s += "\"" + current_node->tag_name + "\"" + ":";
    }
    if (!current_node->have_data) {
      s += tab + "{\n";
    }

    int occurrenceCount = 0;
    int occurrenceIndex = 0;
    bool isArray = false;
    string attributeJson;
    for (auto tag : v) {
      occurrenceCount = 0;
      for (int idx = 0; idx < current_node->num_children; idx++) {
        if (tag == current_node->children[idx]->tag_name) {
          occurrenceCount++;
        }
      }
      for (int idx = 0; idx < current_node->num_children; idx++) {
        if (tag == current_node->children[idx]->tag_name) {
          if (occurrenceIndex == 0 &&
              !current_node->children[idx]->children.empty()) {
            s += tab + "\"" + current_node->children[idx]->tag_name + "\"" +
                 ": ";
            if (current_node->children[idx]->is_tag &&
                !current_node->children[idx]->have_data) {
              for (int j = 0;
                   j < current_node->children[idx]->attributes.size(); j++) {
                attributeJson += "\n" + tab;
                attributeJson +=
                    "\"_" + current_node->children[idx]->attributes[j] + "\"" +
                    ": " + "\"" + current_node->children[idx]->attrib_value[j] +
                    "\"";
                if (j + 1 == current_node->children[idx]->attributes.size()) {
                  attributeJson += "\n";
                } else {
                  attributeJson += ",";
                }
              }
            }
            if ((current_node->children[idx]->is_tag && occurrenceCount > 1)) {
              s += "[\n";
              isArray = true;
            }
          }
          if (current_node->children[idx]->have_data && occurrenceCount > 1) {
            s +=
                tab + convert_json(current_node->children[idx], tab + "      ");
            if (occurrenceIndex < occurrenceCount - 1) {
              s += ",\n";
            } else {
              s += "\n";
            }
          } else if (current_node->children[idx]->have_data &&
                     occurrenceCount == 1) {
            s += convert_json(current_node->children[idx], tab + "      ");
            bool cond = (occurrenceIndex < occurrenceCount - 1);
            if ((occurrenceCount == 1 &&
                 current_node->children[idx]->attributes.size() >= 0 &&
                 v[v.size() - 1] != tag) ||
                cond) {
              s += ",\n";
            } else {
              s += "\n";
            }
          } else {
            s += convert_json(current_node->children[idx], tab + "      ");
            bool cond = (occurrenceIndex < occurrenceCount - 1);

            if (!current_node->children[idx]->getIsComment() &&
                ((occurrenceCount == 1 &&
                  current_node->children[idx]->attributes.size() >= 0 &&
                  v[v.size() - 1] != tag) ||
                 cond)) {
              s += ",\n";
            } else {
              s += "\n";
            }
          }
          occurrenceIndex++;
        }
      }
      if (isArray) {
        if (v[v.size() - 1] != tag) {
          s += tab + "],\n";
        } else {
          s += tab + "]\n";
        }

        isArray = false;
      }
      if (!attributeJson.empty()) {
        s += ",";
        s += attributeJson;
        attributeJson = "";
      }

      occurrenceIndex = 0;
    }
    if (is_root_node) {
      s += tab + "}";
      if (!current_node->attributes.empty()) {
        s += ",";
      }
      for (int i = 0; i < current_node->attributes.size(); i++) {
        s += "\n" + tab;
        s += "\"_" + current_node->attributes[i] + "\"" + ": " + "\"" +
             current_node->attrib_value[i] + "\"";
        if (i + 1 == current_node->attributes.size()) {
          s += "\n";
        } else {
          s += ",";
        }
      }
      s += "}";
      is_root_node = false;
    } else if (!current_node->have_data) {
      s += tab + "}\n";
    }
  } else if (!current_node->attributes.empty()) {
    s += "\n" + tab + "{\n" + tab + R"("__text": ")" +
         current_node->children[0]->tag_name + "\",\n";
    for (int i = 0; i < current_node->attributes.size(); i++) {
      s += tab + "\"_" + current_node->attributes[i] + "\"" + ": " + "\"" +
           current_node->attrib_value[i] + "\"";
      if (i + 1 == current_node->attributes.size()) {
        s += "\n";
      } else {
        s += ",\n";
      }
    }
    s += tab + "}";

  } else {
    s += "\"" + current_node->children[0]->tag_name + "\"";
  }
  return s;
}
