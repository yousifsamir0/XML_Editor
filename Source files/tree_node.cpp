#include "tree.h"
#include <fstream>

// Node-related implementations: construction, attribute parsing storage,
// and simple accessors. Kept minimal and focused on node responsibilities.

Node::Node(const std::string &name, bool is_tag, bool self_close_,
           bool comment_, bool xml) {
  this->tag_name = name;
  this->is_tag = is_tag;
  this->comment = comment_;
  this->self_close = self_close_;
  this->xml = xml;
  have_data = false;
  err_tag = "";
  correct_tag = "";
  num_children = 0;
}

Node::~Node() = default;

auto Node::tagName() const -> std::string { return this->tag_name; }

auto Node::isTag() const -> bool { return this->is_tag; }

void Node::addChild(Node *child) {
  this->children.push_back(child);
  num_children++;
}

void Node::addAttr(const std::string &attribute) {
  int pos = 0;
  std::string name;
  std::string value;
  while (pos < (int)attribute.length()) {
    name.clear();
    value.clear();
    while (pos < (int)attribute.length() && attribute[pos] != '=') {
      name.push_back(attribute[pos]);
      pos++;
    }
    // skip =" sequence if present
    pos += 2;
    while (pos < (int)attribute.length() && attribute[pos] != '"') {
      value.push_back(attribute[pos]);
      pos++;
    }
    // advance past closing '"'
    pos++;
    this->attributes.push_back(name);
    this->attrib_value.push_back(value);
  }

  this->attr = attribute;
}

void Node::setValid(bool statue) { this->is_valid = statue; }

auto Node::getChild() const -> std::vector<Node *> { return this->children; }

void Node::setHaveData() { this->have_data = true; }

auto Node::getHaveData() const -> bool { return this->have_data; }

void Node::setEndTag(std::string tag) { this->err_tag = tag; }

auto Node::getEndTag() const -> std::string { return this->err_tag; }

void Node::setCorrectTag(std::string tag) { this->correct_tag = tag; }

auto Node::getCorrectTag() const -> std::string { return this->correct_tag; }

auto Node::getAttr() const -> std::string { return this->attr; }

auto Node::getIsComment() const -> bool { return this->comment; }

auto Node::getIsSelfClose() const -> bool { return this->self_close; }

// Write prettified XML for the subtree rooted at root to a file.
auto Tree::write_prettify_to_file(const std::string &filepath) -> bool {
  std::ofstream ofs(filepath);
  if (!ofs.is_open()) {
    return false;
  }
  ofs << this->prettify(this->getRoot(), "");
  ofs.close();
  return true;
}
