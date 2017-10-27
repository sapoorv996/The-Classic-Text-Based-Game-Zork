//
//  Item.hpp
//  Zork
//
//  Created by Geoff on 10/16/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef ITEM_H_
#define ITEM_H_

#include <stdio.h>
#include <string>
#include <vector>

#include "rapidxml.hpp"
#include "Trigger.hpp"

using namespace std;
using namespace rapidxml;

typedef struct TurnOn {
    bool has;
    string print;
    string action;
} TurnOn;

class Item {
public:
    Item(xml_node<>* xnode) {setUpItem(xnode);};
    ~Item();
private:
    string name;
    string status;
    string description;
    string writing;
    TurnOn turnon;
    vector<Trigger *> trigger_list;

    void setUpItem(xml_node<>* xnode) {
        turnon.has = false;
    	for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if(string(curr->name()) == "name"){
				name = curr->value();
				cout << "Item - name: " << name << endl;
			} else if (string(curr->name()) == "status") {
				status = curr->value();
			} else if (string(curr->name()) == "description") {
				description = curr->value();
			} else if (string(curr->name()) == "writing") {
				writing = curr->value();
			} else if (string(curr->name()) == "turnon") {
                turnon.has = true;
                for (xml_node<> * curr2 = curr->first_node(); curr2; curr2 = curr2->next_sibling()) {
                    if (string(curr2->name()) == "print") {
                        turnon.print = curr2->value();
                    } else if (string(curr2->name()) == "action") {
                        turnon.action = curr2->value();
                    }
                }
			} else if (string(curr->name()) == "trigger") {
				Trigger * trigger = new Trigger(curr);
				trigger_list.push_back(trigger);
			}
		}
    }
    void setUpTurnOn(xml_node<>*);
};

#endif /* Item_hpp */
