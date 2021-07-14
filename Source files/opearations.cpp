#include <iostream>
#include <vector>
#include<stack>
#include <algorithm> 
#include "XML_Parser.h"

Tree::XML_Parser(string raw_xml)
{
	this->root = NULL;
	this->parse(raw_xml);

}

string Tree::prettify(node* current_node, string tab)
{
	string s="";
	if (current_node->is_valid) {
		s += tab+"<" + current_node->tag_name;
		for (int i = 0; i < current_node->attributes.size(); i++) {
			s += " " + current_node->attributes[i] + "=\"" + current_node->attrib_value[i] + "\"";
		}
		s += ">";
	}

	if (!current_node->is_tag && current_node->is_valid) {
		 s+=(current_node->data);
		 s +=  "</" + current_node->tag_name + ">\n";
		 return s;
	}
	if (current_node->is_tag) { s += "\n"; }
	for (auto i : current_node->children) {
		s += prettify(i,"   "+tab);
	}

	s += tab+"</" + current_node->tag_name + ">\n";
	return s;
}



string Tree::convert_json(node* current_node, string tab,bool print_tag) {
	string s = "";
	if (current_node->is_tag) {
		vector<string> v;
		for (auto i : current_node->children) {
			v.push_back(i->tag_name);
		}
		sort(v.begin(), v.end());
		v.erase(unique(v.begin(), v.end()), v.end());
		
		

		if (current_node == this->root) { 
			s += "{\n";
			s +=   "\"" + current_node->tag_name + "\"" + ":"; 
			
		}

		s += tab+"{\n";

		int counter2= 0;
		int counter = 0;
		bool flag = false;
		bool flag2 = false;
		string attrib = "";
		for (auto tag : v) {
			counter2 = 0;
			for (int i = 0; i < current_node->num_children; i++) {
				if (tag == current_node->children[i]->tag_name) { counter2++; }
			}
			for (int i = 0; i < current_node->num_children;i++) {
				if (tag == current_node->children[i]->tag_name) {
					if (counter==0) {
						s += tab + "\"" + current_node->children[i]->tag_name + "\"" + ": ";
						if (current_node->children[i]->is_tag) {
							for (int j = 0; j < current_node->children[i]->attributes.size(); j++) {
								attrib += "\n" + tab;
								attrib += "\"_" + current_node->children[i]->attributes[j] + "\"" + ": " + "\"" + current_node->children[i]->attrib_value[j] + "\"";
								if (j + 1 == current_node->children[i]->attributes.size()) {
									attrib += "\n";
								}
								else { attrib += ","; }
							}
						}
						if ((current_node->children[i]->is_tag && current_node->children[i]->num_children > 1) || counter2>1) {
							auto x = current_node->children[i];
							auto var = ((current_node->children[i]->is_tag && current_node->children[i]->num_children > 1) || counter2 > 1);
							
							s += "[\n";
							flag = true; 
						}
					}

					if (!current_node->children[i]->is_tag && counter2 > 1) {
						s += tab+convert_json(current_node->children[i], tab + "  ", true);
						if (counter < counter2-1)
							s += ",\n";
						else
							s += "\n";
					}
					else {
						s += convert_json(current_node->children[i], tab + "  ", true);
						bool cond = (counter < counter2-1);
						if ((counter2 == 1 && current_node->children[i]->attributes.size() >= 0 && v[v.size() - 1] != tag) || cond) { s += ",\n"; }
						else { s += "\n"; }
					}
					counter++;
					
				}
			}
			if (flag ) {
				if (v[v.size() - 1] != tag) { s += tab + "],\n"; }
				else { s += tab + "]\n"; }
				
				flag = false;
			}
			if (attrib!="") {
				s += ",";
			}
			s += attrib;
			attrib = "";
			counter=0;
		}
		if (current_node == this->root) {
			s += tab + "}";
			s += ",";
			for (int i = 0; i < current_node->attributes.size(); i++) {
				s += "\n" + tab;
				s += "\"_" + current_node->attributes[i] + "\"" + ": " + "\"" + current_node->attrib_value[i] + "\"";
				if (i + 1 == current_node->attributes.size()) {
					s += "\n";
				}
				else { s += ","; }
			}
			s += "}";
		} 
		else {
			s += tab + "}\n";
		}
	}
	else if(current_node->attributes.size() > 0){
		s += "\n"+ tab + "{\n"+ tab + "\"__text\": \"" + current_node->data + "\",\n";
		for (int i = 0; i < current_node->attributes.size(); i++) {
			s += tab + "\"_" + current_node->attributes[i] + "\"" + ": " + "\"" + current_node->attrib_value[i] + "\"";
			if (i+1== current_node->attributes.size()) { s += "\n"; }
			else { s += ",\n"; }
		}
		s += tab+"}";
	}
	else {
		s +=  "\"" + current_node->data + "\"";
	}
	return s;
}

