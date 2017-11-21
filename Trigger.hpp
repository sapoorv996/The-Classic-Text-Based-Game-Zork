//
//  Trigger.hpp
//  Zork
//
//  Created by Geoff on 10/16/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef TRIGGER_H_
#define TRIGGER_H_

#include <stdio.h>
#include <vector>
#include <string>

#include "rapidxml.hpp"
#include "Condition.hpp"

using namespace std;
using namespace rapidxml;

class Trigger {
public:
    Trigger(xml_node<>* xnode) {setupNode(xnode);}
    ~Trigger();

    void setupNode(xml_node<>* xnode) {
    	has_command = false;
		has_print = false;
		has_action = false;
    command = "";
    type = "single"; //default value is single


    	for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if (string(curr->name()) == "type") {
				type = curr->value();
			} else if (string(curr->name()) == "print") {
				has_print = true;
				print = curr->value();
			} else if (string(curr->name()) == "command") {
				has_command = true;
				command = curr->value();
			} else if (string(curr->name()) == "action") {
				has_action = true;
				action.push_back(string(curr->value()));
			} else if (string(curr->name()) == "condition") {
                Condition* c = new Condition(curr);
                conditions.push_back(c);
			}
		}
    }

    vector<Condition*> conditions;
    string type;
    string command;
    bool has_command;
    string print;
    bool has_print;
    vector <string> action;
    bool has_action;

    // Status * my_s;
    // Owner * my_o;


};

#endif /* Trigger_hpp */
