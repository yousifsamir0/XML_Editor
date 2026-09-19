#include "tree.h"
#include <algorithm>
#include <stack>

#include "constants.h"
#include <fstream>

// Trimming helpers
auto Tree::ltrim(const std::string &s) -> std::string {
  size_t start = s.find_first_not_of(WHITESPACE);
  return (start == std::string::npos) ? "" : s.substr(start);
}

auto Tree::rtrim(const std::string &s) -> std::string {
  size_t end = s.find_last_not_of(WHITESPACE);
  return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}

auto Tree::trim(const std::string &s) -> std::string { return rtrim(ltrim(s)); }

// Attach the closing `tag` to its parent. Uses a small stack to
// recover from mismatched tags: nodes that cannot be matched are
// marked invalid but are still attached so the tree is preserved.
void Tree::allocate_tage_to_parent(std::stack<Node *> *xmlTags,
                                   const std::string &tag) {
  if (!xmlTags->empty()) {

    if (xmlTags->top()->tagName() != tag) {
      std::stack<Node *> my_stk;

      while (!xmlTags->empty() && xmlTags->top()->tagName() != tag) {
        my_stk.push(xmlTags->top());
        xmlTags->pop();
      }
      if (xmlTags->empty()) {

        while (!my_stk.empty()) {
          xmlTags->push(my_stk.top());
          my_stk.pop();
        }
        xmlTags->top()->setValid(false);
        Node *value = xmlTags->top();
        xmlTags->pop();
        xmlTags->top()->addChild(value);
        value->setCorrectTag(value->tagName());
      } else {
        Node *value = xmlTags->top();
        xmlTags->pop();
        xmlTags->top()->addChild(value);
        value->setValid(true);
        value->setCorrectTag(value->tagName());
        while (!my_stk.empty()) {
          Node *value_ = my_stk.top();
          my_stk.pop();
          value_->setValid(false);
          value_->setCorrectTag(value_->tagName());
          value->addChild(value_);
          value = value_;
        }
      }
    }
    if (xmlTags->top()->tagName() == tag) {
      Node *value = xmlTags->top();
      xmlTags->pop();
      if (!xmlTags->empty()) {
        xmlTags->top()->addChild(value);
      }
      value->setValid(true);
      value->setCorrectTag(value->tagName());
    }
  }
}

void Tree::data_node(std::stack<Node *> *xmlTags, Node *data) {

  data->setValid(true);
  xmlTags->top()->addChild(data);

  if (!data->isTag()) {
    xmlTags->top()->setHaveData();
  }
}

// Parse a raw XML string into the tree structure. This parser is
// intentionally simple: it performs a single-pass parse using a
// stack to match opening and closing tags. It assumes reasonably
// well-formed input; error recovery is limited but nodes get
// marked invalid when mismatches occur.
void Tree::parser(const std::string &rawXml) {
  std::stack<Node *> xmlTags;
  std::string tag_name;
  std::string attr_name; // attributes in open tag
  std::string data;
  bool rootInitialized = false;
  for (int i = 0; i < (int)rawXml.length(); i++) {
    if (rawXml[i] == '<') {
      i++;
      while (rawXml[i] != '>') {
        data += rawXml[i];
        i++;
      }
      if (data[0] == '!') // comment
      {
        if (!rootInitialized) {
          this->setRoot(new Node(data, true, false, true, false));
          xmlTags.push(this->getRoot());
          rootInitialized = true;
        } else {
          data_node(&xmlTags, new Node(data, true, false, true, false));
        }
      } else if (data[0] == '?') //?xml
      {
        if (!rootInitialized) {
          this->setRoot(new Node(data, true, false, false, true));
          xmlTags.push(this->getRoot());
          rootInitialized = true;
        } else {
          data_node(&xmlTags, new Node(data, true, false, false, true));
        }
      } else if (data[0] == '/') // end tag
      {
        data = data.substr(1, data.length());
        allocate_tage_to_parent(&xmlTags, data);
      } else if (rawXml[i - 1] == '/') // self close
      {
        if (!rootInitialized) {
          this->setRoot(new Node(data, true, true, false, false));
          xmlTags.push(this->getRoot());
          rootInitialized = true;
        } else {
          data_node(&xmlTags, new Node(data, true, true, false, false));
        }
      } else // open tag
      {
        if (data.find(' ') < 100) {
          tag_name = data.substr(0, data.find(' '));
          attr_name = data.substr(data.find(' ') + 1, data.length());
          Node *open_tag = new Node(tag_name, true, false, false, false);
          open_tag->addAttr(attr_name);
          if (!rootInitialized) {
            this->setRoot(open_tag);
            xmlTags.push(this->getRoot());
            rootInitialized = true;
          } else {
            xmlTags.push(open_tag);
          }
        } else {
          tag_name = data;
          Node *open_tag = new Node(tag_name, true, false, false, false);
          if (!rootInitialized) {
            this->setRoot(open_tag);
            xmlTags.push(this->getRoot());
            rootInitialized = true;
          } else {
            xmlTags.push(open_tag);
          }
        }
      }
      data = "";
    } else {
      while (rawXml[i] != '<' && i != (int)rawXml.size()) {
        data += rawXml[i];
        i++;
      }
      data = trim(data);
      if (!data.empty()) {
        Node *data_node_ = new Node(data, false, false, false, false);
        data_node(&xmlTags, data_node_);
      }
      data = "";
      i--;
    }
  }
}

// Tree lifecycle and basic accessors
Tree::Tree(const std::string &rawXml) { parser(rawXml); }
void Tree::delete_tree(Node *current_node) {
  for (auto *i : current_node->children) {
    delete_tree(i);
  }
  delete current_node;
}
Tree::~Tree() {
  if (root != nullptr) {
    delete_tree(root);
  }
}
void Tree::setRoot(Node *ptr) { this->root = ptr; }
auto Tree::getRoot() -> Node * { return root; }

// Minify implementation remains under parser/render responsibilities
auto Tree::minify(Node *root, int tab) -> std::string {
  std::string s;
  if (root->getChild().empty()) {
    if (!root->isTag()) {
      s += root->tagName();
    } else {
      s += std::string(xml_editor::constants::LT) + root->tagName();
      if (!root->getAttr().empty()) {
        s += (" ") + root->getAttr();
      }
      s += xml_editor::constants::GT;
      if (!root->comment && !root->xml && !root->self_close) {
        s += std::string(xml_editor::constants::LT_SLASH) +
             root->getCorrectTag() + xml_editor::constants::GT;
      }
    }
    return s;
  }
  if (root->getChild().size() == 1 && root->getHaveData()) {
    s += std::string(xml_editor::constants::LT) + root->tagName();
    if (!root->getAttr().empty()) {
      s += (" ") + root->getAttr();
    }
    s += xml_editor::constants::GT;
    s += minify(root->getChild()[0], 0);
    if (!root->comment && !root->xml && !root->self_close) {
      s += std::string(xml_editor::constants::LT_SLASH) +
           root->getCorrectTag() + xml_editor::constants::GT;
    }

  } else {
    s += std::string(xml_editor::constants::LT) + root->tagName();
    if (!root->getAttr().empty()) {
      s += (" ") + root->getAttr();
    }
    s += xml_editor::constants::GT;
    int j = root->getChild().size();
    for (int k = 0; k < j; k++) {
      s += minify(root->getChild()[k], tab);
    }

    if (!root->comment && !root->xml && !root->self_close) {
      s += std::string(xml_editor::constants::LT_SLASH) +
           root->getCorrectTag() + xml_editor::constants::GT;
    }
  }
  return s;
}
auto Tree::minify() -> std::string { return minify(this->root, 1); }

auto Tree::write_minify_to_file(const std::string &filepath) -> bool {
  std::ofstream ofs(filepath);
  if (!ofs.is_open()) {
    return false;
  }
  ofs << this->minify();
  ofs.close();
  return true;
}
