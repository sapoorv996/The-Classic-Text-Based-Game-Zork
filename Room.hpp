#ifndef ROOM_H_
#define ROOM_H_

#include <string>
#include <vector>
#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <list>

using namespace rapidxml;
using namespace std;

class Room
{
public:
	std::string name;
	std::string status; 
	std::string type; 
	std::string description; 
	// vector<Border *> border_arr; 
	// vector<std::string *> container_arr; 
	// vector<std::string *> item_arr;
	// vector<std::sting *> creature_arr; 
	// vector<Trigger *> trigger_arr;

	Room(xml_node<> * xnode) {setupNode(xnode);}
	~Room();

	void setupNode(xml_node<> * xnode) {
		for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if (string(curr->name()) == "name") {
				name = curr->value();
				cout << "Room - name: " << name << endl;
			}else if (string(curr->name()) == "status"){
				status = curr->value();
			}else if (string(curr->name()) == "type"){
				type = curr->value();
			}else if (string(curr->name()) == "description"){
				description = curr->value();
			}
		}
	}
};
#endif /* ROOM_H_ */