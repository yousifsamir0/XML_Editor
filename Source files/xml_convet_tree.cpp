#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stack>
#include <new>
#include "tree.h"
using namespace std;
using std::cout; using std::cerr;
using std::endl; using std::string;
using std::ifstream; using std::ostringstream;
node::node(string name, bool is_tag, bool self_close_, bool comment_)
{
    this->tag_name = name;
    this->is_tag = is_tag;
    this->comment = comment_;
    this->self_close = self_close_;
    have_data = false;
    err_tag = "";
    correct_tag = "";
}

node::~node()
{

}

string node::tagName()
{
    return this->tag_name;
}
bool node::isTag()
{
    return this->is_tag;
}
void node::addChild(node* child) {
    this->children.push_back(child);
}
void node::addAtrr(string attrbiute)
{
    this->attr = attrbiute;
}
void node::setValid(bool statue)
{
    this->is_valid = statue;
}
vector<node*> node::getChild()
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
void node::setEndTag(string tag)
{
    this->err_tag = tag;
}
string node::getEndTag()
{
    return this->err_tag;
}
void node::setCorrectTag(string tag)
{
    this->correct_tag = tag;
}
string node::getCorrectTag()
{
    return this->correct_tag;
}
string node::getAttr()
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
void node::setState(bool state)
{
    this->state = state;
}
bool node::getState()
{
    return this->state;
}





string readFileIntoString(const string& path) {
    ifstream input_file(path);
    if (!input_file.is_open()) {
        cerr << "Could not open the file - '"
            << path << "'" << endl;
        exit(EXIT_FAILURE);
    }
    return string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

void Tree::allocate_tage_to_parent(stack<node*>* xmlTags, string tag)
{
    if (xmlTags->size() > 1) {

        if (xmlTags->top()->tagName() != tag)
        {
            stack<node*> my_stk;

            while (xmlTags->size() > 1 && xmlTags->top()->tagName() != tag)
            {
                my_stk.push(xmlTags->top());
                xmlTags->pop();
            }
            if (xmlTags->size() > 1 && xmlTags->top()->tagName() == tag)
            {
                while (!my_stk.empty()) {
                    xmlTags->top()->addChild(my_stk.top());
                    my_stk.top()->setValid(false);
                    my_stk.pop();
                }
                xmlTags->top()->setValid(true);
                node* value = xmlTags->top();
                xmlTags->pop();
                xmlTags->top()->addChild(value);
            }
            else {
                while (!my_stk.empty()) {
                    xmlTags->push(my_stk.top());
                    my_stk.pop();
                }
                xmlTags->top()->setValid(false);
                node* value = xmlTags->top();
                xmlTags->pop();
                xmlTags->top()->addChild(value);

            }

        }
        if (xmlTags->top()->tagName() == tag)
        {
            node* value = xmlTags->top();
            xmlTags->pop();
            xmlTags->top()->addChild(value);
            value->setValid(true);
            value->setEndTag(tag);

        }
    }


}
void Tree::data_node(stack<node*>* xmlTags, node* data, bool state) {
    data->setValid(true);
    xmlTags->top()->addChild(data);

    if (!data->isTag()) {
        xmlTags->top()->setHaveData();
        xmlTags->top()->setState(state);
    }
}
void Tree::parser(string rawXml)
{
    stack<node*> xmlTags;
    int start_attr;
    string tag_name = "";
    string end_tag = "";
    string attr_name = ""; //attributes in open tag
    bool first_space = false;
    string data = "";
    int r = 0;
    bool state = false;
    for (int i = 0; i < rawXml.length(); i++)
    {
        if (rawXml[i] == '<' && rawXml[i + 1] != '/')
        {
            if (rawXml[i + 1] == '!')
            {
                i++;
                while (rawXml[i] != '>')
                {
                    end_tag += rawXml[i];
                    i++;
                }
                node* comment_ = new node(end_tag, true, true, false);
                data_node(&xmlTags, comment_, false);
                end_tag = "";
            }
            else if (rawXml[i + 1] == '?')
            {

                i++;
                while (rawXml[i] != '>')
                {
                    end_tag += rawXml[i];
                    i++;
                }

                node* self_close_ = new node(end_tag, true, false, true);
                if (r == 0) {
                    this->setRoot(self_close_);
                    xmlTags.push(self_close_);
                }
                else
                    data_node(&xmlTags, self_close_, false);
                r++;
                end_tag = "";
            }
            else
            {
                i++;
                while (rawXml[i] != '>')
                {
                    while (rawXml[i] != ' ' && rawXml[i] != '>' && first_space == false)
                    {

                        tag_name += rawXml[i];
                        i++;
                    }
                    first_space = true;
                    rawXml[i] == ' ' ? i++ : i;
                    if (rawXml[i] == '>')
                    {
                        break;
                    }
                    while (rawXml[i] != '>' && first_space == true)
                    {
                        attr_name += rawXml[i];
                        i++;
                    }
                }
                if (rawXml[i - 1] == '/')
                {
                    node* self_close_ = new node(tag_name + " " + attr_name, true, true, false);
                    data_node(&xmlTags, self_close_, false);
                }
                else
                {
                    node* openTag = new node(tag_name, true, false, false);
                    openTag->addAtrr(attr_name);
                    if (xmlTags.size() > 1)
                        openTag->setCorrectTag(tag_name);
                    xmlTags.push(openTag);
                }
                tag_name = "";
                attr_name = "";
                first_space = false;
            }
        }
        if (rawXml[i] != '>' && rawXml[i] != '<')
        {
            while (rawXml[i] != '<')
            {
                data += rawXml[i];
                i++;
            }
            if (rawXml[i + 1] != '/')
                state = true;
            else
                state = false;
            node* dataNode = new node(data, false, false, false);
            data_node(&xmlTags, dataNode, state);
            state = false;
            data = "";
            i--;
        }
        if (rawXml[i] == '>') continue;

        if (rawXml[i] == '<' && rawXml[i + 1] == '/')
        {
            i += 2;
            while (rawXml[i] != '>')
            {
                end_tag += rawXml[i];
                i++;
            }
            allocate_tage_to_parent(&xmlTags, end_tag);
            end_tag = "";
        }
    }
}

void traverse(node* root)
{
    if (root->getChild().size()==0) {
        cout << root->tagName() << " " << root->getAttr() << root->getEndTag() << " ";
        return;
    }
    cout << root->tagName() << " " << root->getAttr();
    int j = root->getChild().size();
    for (int i = 0; i < j; i++)
        traverse(root->getChild()[i]);
    cout <<root->getCorrectTag()<<endl;
}

int main()
{
    string filename("xml2.xml");//put file name
    string file_contents;
    file_contents = readFileIntoString(filename);
    Tree* tree = new Tree();
    tree->parser(file_contents);
    traverse(tree->getRoot());
  
    
    return 0;
}
