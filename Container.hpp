#ifndef CONTAINER_H_
#define CONTAINER_H_

#include <string>
#include <vector>
#include <list>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

#include "Trigger.hpp"
#include "Item.hpp"

using namespace rapidxml;
using namespace std;

class Container
{
public:
	string name;
	string status;
	string description;
	vector<string> valid_list;
	vector<string> item_list;
	vector<Trigger *> trigger_list;

	Container(xml_node<> * xnode) {setupNode(xnode);}
	~Container();

	void setupNode(xml_node<> * xnode) {
		for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if (string(curr->name()) == "name") {
				name = curr->value();
				cout << "Container - name: " << name << endl;
			}else if (string(curr->name()) == "status"){
				status = curr->value();
			}else if (string(curr->name()) == "description"){
				description = curr->value();
			}else if (string(curr->name()) == "accept"){
				valid_list.push_back(curr->value());
			}else if(string(curr->name()) == "item"){
				item_list.push_back(curr->value());
			}else if(string(curr->name()) == "trigger"){
				Trigger * trigger = new Trigger(curr);
				trigger_list.push_back(trigger);
			}
		}
	}

	bool is_accepted_by_container(string obj) {
		if (valid_list.size() == 0){
			return true;
		}

		for (unsigned int i = 0; i < valid_list.size(); i++){
			if (valid_list[i] == obj){
				return true;
			}
		}

		return false;
	}

	bool has_item(string obj) {
		for (int i = 0; i < item_list.size(); i++){
			if (item_list[i] == obj){
				return true;
			}
		}
		
		return false;
	}
};

#endif /* CREATURE_H_ */