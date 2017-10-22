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

using namespace std;
using namespace rapidxml;

typedef struct {
    string object;
    string status;
}Status;

typedef struct {
    string object;
    string has;
    string owner;
}Owner;

class Trigger {
public:
    Trigger(xml_node<>* xnode) {setupNode(xnode);}
    ~Trigger();

    void setupNode(xml_node<>* xnode) {
    	bool has_command = false;
		bool has_print = false;
		bool has_action = false;

    	for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if (string(curr->name()) == "type"){
				type = curr->value();
			}else if (string(curr->name()) == "print"){
				has_print = true;
				print = curr->value();
			}else if (string(curr->name()) == "command"){
				has_command = true;
				command = curr->value();
			}else if (string(curr->name()) == "action"){
				has_action = true;
				action.push_back(string(curr->value()));
			}else if (string(curr->name()) == "condition"){
				int condition = condition_count(curr);
				if(condition == 2) {
					setupStatus(curr);
				} else if(condition == 3) {
					setupOwner(curr);
				}
			}
		}
    }

    int condition_count(xml_node<>* xnode) {
    	int num = 0;
        for(xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()) {
            num++;
        }
        return num;
    }

    void setupStatus(xml_node<>* xnode) {
        for(xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()) {
        	if(string(curr->name()) == "object"){
                s.object = curr -> value();
            }
            if(string(curr->name()) == "status"){
                s.status = curr -> value();
            }
        }	
    }

    void setupOwner(xml_node<>* xnode) {
        for(xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()) {
        	if(string(curr->name()) == "object"){
                o.object = curr -> value();
            }
            if(string(curr->name()) == "has"){
                o.has = curr -> value();
            }
            if(string(curr->name()) == "owner"){
                o.owner = curr -> value();
            }
        }	
    }

private:
    // vector<Condition*> conditions;
    string type;
    string command;
    bool has_command;
    string print;
    bool has_print;
    vector <string> action;
    bool has_action;

    Status s;
    Owner o;
};

#endif /* Trigger_hpp */
