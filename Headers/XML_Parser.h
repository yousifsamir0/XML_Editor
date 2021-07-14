#ifndef XML_PARSER_H
#define XML_PARSER_H

#include "node.h"

class XML_Parser
{
private:
	node* root;
	string raw_xml;
public:
	XML_Parser(string raw_xml);
	~XML_Parser();
	node* parse(string raw_xml);
	node* get_root() { return this->root; }
	vector<node*> check_consistency(vector<node*>& inValidNodes, node* current_node);
	string solve_errors(vector<node*>);
	string prettify(node* r, string tab);
	string convert_json(node* current_node, string tab, bool print_tag);
	string minify();
	void traverse(node* root);
};
#endif
