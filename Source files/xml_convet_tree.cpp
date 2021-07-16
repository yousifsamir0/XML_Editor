#include <iostream>
#include "tree.h"



node::node(std::string name, bool is_tag, bool self_close_, bool comment_, bool xml)
{
    this->tag_name = name;
    this->is_tag = is_tag;
    this->comment = comment_;
    this->self_close = self_close_;
    this->xml = xml;
    have_data = false;
    err_tag = "";
    correct_tag = "";
    num_children=0;
}

node::~node()
{
	
}

std::string node::tagName()
{
	return this->tag_name;
}
bool node::isTag()
{
	return this->is_tag;
}
void node::addChild(node* child) {
	this->children.push_back(child);
	num_children++;
}
void node::addAtrr(std::string attrbiute)
{
    int i = 0;
    std::string name="";
    std::string value="";
    while (i < attrbiute.length()) {
        name = "";
        value = "";
        while (attrbiute[i] != '=') {
            name += attrbiute[i];
            i++;
        }
        i+=2;
        while (attrbiute[i] != '"' && i < attrbiute.length()) {
            value += attrbiute[i];
            i++;
        }
        i++;
        this->attributes.push_back(name);
        this->attrib_value.push_back(value);
    }

    this->attr = attrbiute;
}
void node::setValid(bool statue)
{
	this->is_valid = statue;
}
std::vector<node*> node::getChild()
{
	return this->children;
}
void node::setHaveData()
{
	this->have_data = true;
}
bool node::getHaveData()
{
	return this->have_data;
}
void node::setEndTag(std::string tag)
{
	this->err_tag = tag;
}
std::string node::getEndTag()
{
	return this->err_tag;
}
void node::setCorrectTag(std::string tag)
{
	this->correct_tag = tag;
}
std::string node::getCorrectTag()
{
	return this->correct_tag;
}
std::string node::getAttr()
{
	return this->attr;
}
bool node::getIsComment()
{
	return this->comment;
}
bool node::getIsSelfClose()
{
	return this->self_close;
}





void Tree::allocate_tage_to_parent(std::stack<node*>* xmlTags, std::string tag)
{
    if (!xmlTags->empty()) {

        if (xmlTags->top()->tagName() != tag)
        {
            std::stack<node*> my_stk;

            while (!xmlTags->empty() && xmlTags->top()->tagName() != tag)
            {
                my_stk.push(xmlTags->top());
                xmlTags->pop();
            }
            if (xmlTags->empty())
            {

                while (!my_stk.empty()) {
                    xmlTags->push(my_stk.top());
                    my_stk.pop();
                }
                xmlTags->top()->setValid(false);
                node* value = xmlTags->top();
                xmlTags->pop();
                xmlTags->top()->addChild(value);
                value->setCorrectTag(value->tagName());
            }
            else {
                node* value = xmlTags->top();
                xmlTags->pop();
                xmlTags->top()->addChild(value);
                value->setValid(true);
                value->setCorrectTag(value->tagName());
                while (!my_stk.empty()) {
                    node* value_ = my_stk.top();
                    my_stk.pop();
                    value_->setValid(false);
                    value_->setCorrectTag(value_->tagName());
                    value->addChild(value_);
                    value = value_;
                }
            }

        }
        if (xmlTags->top()->tagName() == tag)
        {
            node* value = xmlTags->top();
            xmlTags->pop();
            if (!xmlTags->empty())
                xmlTags->top()->addChild(value);
            value->setValid(true);
            value->setCorrectTag(value->tagName());
        }
    }
}

void Tree::data_node(std::stack<node*>* xmlTags, node* data) {


    data->setValid(true);
    xmlTags->top()->addChild(data);


    if (!data->isTag()) {
        xmlTags->top()->setHaveData();
    }
}


std::string Tree::ltrim(const std::string& s)
{
    size_t start = s.find_first_not_of(WHITESPACE);
    return (start == std::string::npos) ? "" : s.substr(start);
}

std::string Tree::rtrim(const std::string& s)
{
    size_t end = s.find_last_not_of(WHITESPACE);
    return (end == std::string::npos) ? "" : s.substr(0, end + 1);
}


std::string Tree::trim(const std::string& s) {
    return rtrim(ltrim(s));
}


void Tree::parser(std::string rawXml)
{
    std::stack<node*> xmlTags;
    int start_attr;
    std::string tag_name = "";
    std::string attr_name = ""; //attributes in open tag
    std::string data = "";
    int r = 0;
    for (int i = 0; i < rawXml.length(); i++)
    {
        if (rawXml[i] == '<')
        {
            i++;
            while (rawXml[i] != '>')
            {
                data += rawXml[i];
                i++;
            }
            if (data[0] == '!')//comment
            {
                if (r == 0)
                {
                    this->setRoot(new node(data, true, false, true, false));
                    xmlTags.push(this->getRoot());
                    r = 1;
                }
                else
                {
                    data_node(&xmlTags, new node(data, true, false, true, false));
                }
            }
            else if (data[0] == '?')//?xml
            {
                if (r == 0)
                {
                    this->setRoot(new node(data, true, false, false, true));
                    xmlTags.push(this->getRoot());
                    r = 1;
                }
                else
                {
                    data_node(&xmlTags, new node(data, true, false, false, true));
                }
            }
            else if (data[0] == '/')//end tag
            {
                data = data.substr(1, data.length());
                allocate_tage_to_parent(&xmlTags, data);
            }
            else if (rawXml[i - 1] == '/')//self close
            {
                if (r == 0)
                {
                    this->setRoot(new node(data, true, true, false, false));
                    xmlTags.push(this->getRoot());
                    r = 1;
                }
                else
                {
                    data_node(&xmlTags, new node(data, true, true, false, false));
                }
            }
            else//open tag
            {
                if (data.find(' ') < 100) {
                    tag_name = data.substr(0, data.find(' '));
                    attr_name = data.substr(data.find(' ') + 1, data.length());
                    node* open_tag = new node(tag_name, true, false, false, false);
                    open_tag->addAtrr(attr_name);
                    if (r == 0)
                    {
                        this->setRoot(open_tag);
                        xmlTags.push(this->getRoot());
                        r = 1;
                    }
                    else
                    {
                        xmlTags.push(open_tag);
                    }
                }
                else {
                    tag_name = data;
                    node* open_tag = new node(tag_name, true, false, false, false);
                    if (r == 0)
                    {
                        this->setRoot(open_tag);
                        xmlTags.push(this->getRoot());
                        r = 1;
                    }
                    else
                    {
                        xmlTags.push(open_tag);
                    }
                }
            }
            data = "";
        }
        else
        {
            while (rawXml[i] != '<' && i!=rawXml.size())
            {
                data += rawXml[i];
                i++;
            }
            data = trim(data);
            if (data != "")
            {
                node* data_node_ = new node(data, false, false, false, false);
                data_node(&xmlTags, data_node_);
            }
            data = "";
            i--;
        }
    }
}

std::string Tree::minify(node* root, int tab)
{
    std::string s="";
    if (root->getChild().size() == 0) {
        if (!root->isTag())
            //cout << root->tagName();
            s+=root->tagName();
        else {
            //cout << '<' + root->tagName() << '>' << "</" << root->getCorrectTag() << ">";
            s+='<' + root->tagName();
	    if(root->getAttr()!="")
		    s+=(" ")+root->getAttr();
	    s+='>';
            if (!root->comment&&!root->xml&&!root->self_close)
                s+= "</" + root->getCorrectTag() + ">";
        }
        return s;
    }
    else if (root->getChild().size() == 1 && root->getHaveData())
    {
        //cout << '<' + root->tagName() << '>';
        s+='<' + root->tagName();
	 if(root->getAttr()!="")
		    s+=(" ")+root->getAttr();
	    s+='>';
        s+=minify(root->getChild()[0], 0);
        //cout << "</" << root->getCorrectTag() << ">";
        if (!root->comment&&!root->xml&&!root->self_close)
            s+="</" + root->getCorrectTag() + ">";

    }
    else
    {
        //cout << '<' + root->tagName() << '>' ;
        s+='<' + root->tagName();
	if(root->getAttr()!="")
		    s+=(" ")+root->getAttr();
	    s+='>';
        int j = root->getChild().size();
        for (int k = 0; k < j; k++)
            s+=minify(root->getChild()[k], tab);

        //cout << "</" << root->getCorrectTag() << ">";
        if (!root->comment&&!root->xml&&!root->self_close)
                s+="</" + root->getCorrectTag() + ">";

    }
    return s;

}
std::string Tree::minify(){
    return minify(this->root,1);
}



Tree::Tree(std::string rawXml){
    parser(rawXml);
}
void Tree::delete_tree(node *current_node){
    for (auto i : current_node->children) {
        delete_tree(i);
    }
    delete current_node;
}
Tree::~Tree(){
    delete_tree(root);
}
void Tree::setRoot(node* ptr){
    this->root=ptr;
}
node* Tree:: getRoot(){
    return root;
}
