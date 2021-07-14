#ifndef NODE_H
#define NODE_H
class node
{
private:
	vector<node*> children;
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

public:
	node(string name, bool is_tag, bool self_close_, bool comment_);
	~node();
	void addChild(node* child);
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
	vector<node*> getChild();
	string tagName();
	bool getIsComment();
	bool getIsSelfClose();

	friend class XML_Parser;
};
#endif
