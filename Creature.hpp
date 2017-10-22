#ifndef CREATURE_H_
#define CREATURE_H_

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

class Creature
{
public:
	string name;
	string status;
	string description;
	vector<string> vulnerability;
	// CreatureAttack *attack;
	// vector<Trigger *> trigger_list;

	Creature(xml_node<> * xnode) {setupNode(xnode);}
	~Creature();

	void setupNode(xml_node<> * xnode) {
		for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if (string(curr->name()) == "name") {
				name = curr->value();
				cout << "Creature - name: " << name << endl;
			}else if (string(curr->name()) == "status"){
				status = curr->value();
			}else if (string(curr->name()) == "description"){
				description = curr->value();
			}else if (string(curr->name()) == "vulnerability"){
				vulnerability.push_back(curr->value());
			}
		}
	}	
};

#endif /* CREATURE_H_ */