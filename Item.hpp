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

using namespace std;
using namespace rapidxml;

class Item {
public:
    Item(xml_node<>* xnode) {setUpItem(xnode);};
    ~Item();
private:
    string name;
    string status;
    string description;
    string writing;
    vector<string> turnon;
    
    void setUpItem(xml_node<>* xnode) {
    	for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if(string(curr->name()) == "name"){
				name = curr->value();
				cout << "Item - name: " << name << endl;
			}else if (string(curr->name()) == "status"){
				status = curr->value();
			}else if (string(curr->name()) == "description"){
				description = curr->value();
			}else if (string(curr->name()) == "writing"){
				writing = curr->value();
			}else if (string(curr->name()) == "turnon"){
				//append to turn on list??
			}else if (string(curr->name()) == "trigger"){
				//Create new trigger object
				//append object to tigger list
			}
		}
    }
    void setUpTurnOn(xml_node<>*);
};

#endif /* Item_hpp */
