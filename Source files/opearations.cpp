string Tree::prettify(node* current_node, string tab)
{

	string s = "";
	bool flag = false;
	bool is_header = current_node->tag_name.substr(0, 4) == "?xml";
	if (current_node->getIsComment() && !is_header) {
		s += tab + "<" + current_node->tag_name + ">";
	}
	else if (current_node->getIsSelfClose() && !is_header) {
		s += tab + "<" + current_node->tag_name + " " + current_node->attr + ">" + "";
	}

	else if (current_node->is_tag && !is_header) {
		s += tab + "<" + current_node->tag_name + " ";
		s += current_node->attr;
		s += ">";
		flag = true;
	}
	else if (!is_header) {
		if (current_node->children.size() == 0) {
			s += (current_node->tag_name);
		}
		else {
			s += "\n";
			s += (current_node->tag_name);
		}

	}

	if (current_node->is_tag && !current_node->getHaveData() && !is_header) { s += "\n"; }

	for (int i = 0; i < current_node->children.size(); i++) {
		if (is_header || current_node->children[i]->getIsComment()) {
			s += prettify(current_node->children[i], tab);
		}
		else {
			s += prettify(current_node->children[i], "      " + tab);
		}
	}
	if (flag && current_node->getHaveData() && current_node->is_valid) {

		s += "</" + current_node->tag_name + ">\n";
		flag = false;
	}
	else if (flag && current_node->is_valid) {
		s += tab + "</" + current_node->tag_name + ">\n";
	}

	else if (flag && current_node->getHaveData() && current_node->children.size() == 0) {
		s += "</" + current_node->getCorrectTag() + ">\n";
		flag = false;
	}
	else if (flag) {
		s += tab + "</" + current_node->tag_name + ">\n";
		flag = false;
	}


	return s;
}



string Tree::convert_json(node * current_node, string tab) {
	string s = "";
	bool is_root_node = (current_node == this->getRoot());
	if (current_node->getIsComment()) {
		return s;
	}
	else if (current_node->getIsSelfClose()) {
		int i = 0;
		s += tab + "\"";
		while (current_node->tag_name[i] != ' ') {
			s += current_node->tag_name[i];
			i++;
		}
		i++;
		s += "\":{";
		while (current_node->tag_name[i] != '/') {
			s += "\"_";

			while (current_node->tag_name[i] != '=') {

				s += current_node->tag_name[i];
				i++;
			}
			i++;
			s += "\":";
			while (current_node->tag_name[i] != ' ' && current_node->tag_name[i] != '/') {
				s += current_node->tag_name[i];
				i++;
			}
			if (current_node->tag_name[i] == ' ') {
				i++;
			}
		}
		s += "}";

		return s;

	}
	else if (current_node->xml) {
		for (auto i : current_node->children) {
			if (i->tag_name.substr(0, 4) != "?xml") {
				current_node = i;
				is_root_node = true;
			}
		}
	}
	if (current_node->is_tag && !current_node->getHaveData()) {
		vector<string> v;
		for (auto i : current_node->children) {
			v.push_back(i->tag_name);
		}
		sort(v.begin(), v.end());
		v.erase(unique(v.begin(), v.end()), v.end());
		if (is_root_node) {
			s += "{\n";
			s += "\"" + current_node->tag_name + "\"" + ":";
		}
		if (!current_node->have_data) {
			s += tab + "{\n";
		}


		int counter2 = 0;
		int counter = 0;
		bool flag = false;
		bool flag2 = false;
		bool attrib_node = false;
		string attrib = "";
		for (auto tag : v) {
			counter2 = 0;
			for (int i = 0; i < current_node->num_children; i++) {
				if (tag == current_node->children[i]->tag_name) { counter2++; }
			}
			for (int i = 0; i < current_node->num_children; i++) {
				if (tag == current_node->children[i]->tag_name) {
					if (counter == 0 && current_node->children[i]->children.size() != 0) {
						s += tab + "\"" + current_node->children[i]->tag_name + "\"" + ": ";
						if (current_node->children[i]->is_tag && !current_node->children[i]->have_data) {

							for (int j = 0; j < current_node->children[i]->attributes.size(); j++) {
								attrib += "\n" + tab;
								attrib += "\"_" + current_node->children[i]->attributes[j] + "\"" + ": " + "\"" + current_node->children[i]->attrib_value[j] + "\"";
								if (j + 1 == current_node->children[i]->attributes.size()) {
									attrib += "\n";
								}
								else {
									attrib += ",";
								}
							}
						}
						if ((current_node->children[i]->is_tag && counter2 > 1)) {
							s += "[\n";
							flag = true;
						}
					}
					auto ex = current_node->children[i];
					if (current_node->children[i]->have_data && counter2 > 1) {
						s += tab + convert_json(current_node->children[i], tab + "      ");
						if (counter < counter2 - 1)
							s += ",\n";
						else
							s += "\n";
					}
					else if (current_node->children[i]->have_data && counter2 == 1) {
						s += convert_json(current_node->children[i], tab + "      ");
						bool cond = (counter < counter2 - 1);
						if ((counter2 == 1 && current_node->children[i]->attributes.size() >= 0 && v[v.size() - 1] != tag) || cond) {
							s += ",\n";
						}
						else { s += "\n"; }
					}
					else {
						s += convert_json(current_node->children[i], tab + "      ");
						//cout << s << endl;
						//cout << "============" << endl;
						bool cond = (counter < counter2 - 1);

						if (!current_node->children[i]->getIsComment() && ((counter2 == 1 && current_node->children[i]->attributes.size() >= 0 && v[v.size() - 1] != tag) || cond)) { s += ",\n"; }
						else { s += "\n"; }
					}
					counter++;

				}
			}
			auto y = true;
			if (flag) {
				if (v[v.size() - 1] != tag) {
					s += tab + "],\n";
				}
				else {
					s += tab + "]\n";
				}

				flag = false;
			}
			if (attrib != "") {
				s += ",";
				s += attrib;
				attrib = "";
			}

			counter = 0;
		}
		if (is_root_node) {
			s += tab + "}";
			if (current_node->attributes.size() > 0)
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
			is_root_node = false;
		}
		else if (!current_node->have_data) {
			s += tab + "}\n";
		}
	}
	else if (current_node->attributes.size() > 0) {
		s += "\n" + tab + "{\n" + tab + "\"__text\": \"" + current_node->children[0]->tag_name + "\",\n";
		for (int i = 0; i < current_node->attributes.size(); i++) {
			s += tab + "\"_" + current_node->attributes[i] + "\"" + ": " + "\"" + current_node->attrib_value[i] + "\"";
			if (i + 1 == current_node->attributes.size()) { s += "\n"; }
			else { s += ",\n"; }
		}
		s += tab + "}";

	}
	else {
		s += "\"" + current_node->children[0]->tag_name + "\"";
	}
	return s;
}
