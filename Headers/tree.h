#ifndef TREE_H
#define TREE_H

#include <stack>
#include <string>
#include <vector>

class Node {
public:
  std::vector<Node *> children;
  std::string tag_name;
  bool is_tag{false};
  bool is_valid{false};
  bool have_data{false};
  std::string err_tag;
  std::string correct_tag;
  std::string attr;
  std::string data;
  bool self_close{false};
  bool comment{false};
  std::vector<std::string> attributes;
  std::vector<std::string> attrib_value;
  int num_children{0};
  bool xml{false};
  Node(const std::string &name, bool is_tag_, bool self_close_, bool comment_,
       bool xml_);
  ~Node();
  void addChild(Node *child);
  void addAttr(const std::string &attribute);
  bool isTag() const;
  std::string getAttr() const;
  void setValid(bool statue);
  void setHaveData();
  bool getHaveData() const;
  void setEndTag(std::string tag);
  std::string getEndTag() const;
  void setCorrectTag(std::string tag);
  std::string getCorrectTag() const;
  std::vector<Node *> getChild() const;
  std::string tagName() const;
  bool getIsComment() const;
  bool getIsSelfClose() const;

  friend class Tree;
};

class Tree {
private:
  Node *root{nullptr};
  std::string raw_xml;
  const std::string WHITESPACE = " \n\r\t\f\v";

public:
  // Constants
  inline static const std::string INDENT = "      ";
  inline static const std::string JSON_ATTR_PREFIX = "_";

  std::string ltrim(const std::string &s);
  std::string rtrim(const std::string &s);
  std::string trim(const std::string &s);
  static void data_node(std::stack<Node *> *xmlTags, Node *data);

public:
  Tree(const std::string &rawXml);
  Tree() = default;
  void delete_tree(Node *current_node);
  ~Tree();
  void setRoot(Node *ptr);
  Node *getRoot();

  std::string get_json() { return this->convert_json(this->root, ""); }
  std::string get_pretty() { return this->prettify(this->root, ""); }

  static void allocate_tage_to_parent(std::stack<Node *> *xmlTags,
                                      const std::string &tag);
  void parser(const std::string &rawXml);
  std::string minify(Node *root, int tab);
  std::string minify();
  std::string prettify(Node *r, std::string tab);
  std::string prettify();
  std::string convert_json(Node *current_node, std::string tab);
  std::string convert_json();
  // File helpers
  bool write_prettify_to_file(const std::string &filepath);
  bool write_minify_to_file(const std::string &filepath);
  bool write_json_to_file(const std::string &filepath);
};

#endif // TREE_H
