#ifndef TREE_H
#define TREE_H
#include <vector>
#include <string>
#include <stack>
#include <new>
using namespace std;
class node
{
public:
    vector<node *> children;
    string tag_name;
    bool is_tag;
    bool is_valid;
    bool have_data;
    string err_tag, correct_tag;
    string attr;
    string data;
    bool self_close;
    bool comment;
    vector<string> attributes;
    vector<string> attrib_value;
    int num_children;
    bool xml;

    node(string name, bool is_tag, bool self_close_, bool comment_, bool xml);
    ~node();
    void addChild(node *child);
    void set_attribute(string attribute);
    bool isTag();
    void addAtrr(string attrbiute);
    string getAttr();
    void setValid(bool statue);
    void setHaveData();
    bool getHaveData();
    void setCorrectTag(string tag);
    string getCorrectTag();
    vector<node *> getChild();
    string tagName();
    bool getIsComment();
    bool getIsSelfClose();

    friend class Tree;
};

class Tree
{
private:
    node *root;
    string raw_xml;
    string WHITESPACE = " \n\r\t\f\v";
    std::string ltrim(const std::string& s);
    std::string rtrim(const std::string& s);
    std::string trim(const std::string& s);
    void data_node(std::stack<node*>* xmlTags, node* data);

public:
    Tree(string rawXml);
    Tree(){};
     void delete_tree(node* current_node);
    ~Tree();
    void setRoot(node *ptr);
    node *getRoot();
    string get_json() {
	    return this->convert_json(this->root, "");
    }
    string get_pretty() {
	return this->prettify(this->root, "");
    }
    void allocate_tage_to_parent(stack<node *> *xmlTags, string tag);
    void parser(string rawXml);
    string minify(node* root, int tab);
    string minify();
    string prettify(node *r, string tab);
    string prettify();
    string convert_json(node *current_node, string tab, bool print_tag);
};
#endif
