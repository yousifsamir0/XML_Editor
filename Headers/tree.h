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
    bool state;
    vector<string> attributes;
    vector<string> attrib_value;
    int num_children;

    node(string name, bool is_tag, bool self_close_, bool comment_);
    ~node();
    void addChild(node *child);
    void set_attribute(string attribute);
    bool isTag();
    void addAtrr(string attrbiute);
    string getAttr();
    void setValid(bool statue);
    void setHaveData();
    bool getHaveData();
    void setState(bool state);
    bool getState();
    void setEndTag(string tag);
    string getEndTag();
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

public:
    Tree()
    {
        root = NULL;
	parser(this->raw_xml);
    }
    void delete_tree(node* current_node) {
		for (auto i : current_node->children) {
			delete_tree(i);
		}
		delete current_node;
	}
    ~Tree()
    {
        delete_tree(this->root);
    }
    void setRoot(node *ptr)
    {
        this->root = ptr;
    }
    node *getRoot()
    {
        return this->root;
    }
    string get_json() {
	    return this->convert_json(this->root, "");
    }
    string get_pretty() {
	return this->prettify(this->root, "");
    }
    void allocate_tage_to_parent(stack<node *> *xmlTags, string tag);
    void data_node(stack<node *> *xmlTags, node *data, bool state);
    void parser(string rawXml); // function to call
    string minify(string raw_Xml);
    string prettify(node *r, string tab);
    string convert_json(node *current_node, string tab, bool print_tag);
};
#endif
