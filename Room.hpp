#ifndef ROOM_H_
#define ROOM_H_

#include <string>
#include <vector>
#include <iostream>

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"

#include "Creature.hpp"
#include "Container.hpp"
#include "Item.hpp"
#include "Room.hpp"

using namespace rapidxml;
using namespace std;

typedef struct {
  string direction;
  string name;
} Border;

class Room
{
public:
	string name;
	string status; 
	string type; 
	string description; 
	vector<Border *> border_arr;
	vector<string> container_arr; 
	vector<string> item_arr;
	vector<string> creature_arr; 
	vector<Trigger *> trigger_arr;

	Room(xml_node<> * xnode) {setupNode(xnode);}
	~Room();

	void setupNode(xml_node<> * xnode) {
		for(xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
			if (string(curr->name()) == "name") {
				name = curr->value();
				cout << "Room - name: " << name << endl;
			}else if (string(curr->name()) == "status"){
				status = curr->value();
			}else if (string(curr->name()) == "type"){
				type = curr->value();
			}else if (string(curr->name()) == "description"){
				description = curr->value();
			}else if (string(curr->name()) == "container"){
				container_arr.push_back(curr->value());
			}else if (string(curr->name()) == "item"){
				item_arr.push_back(curr->value());
			}else if (string(curr->name()) == "creature"){
				creature_arr.push_back(curr->value());
			}else if (string(curr->name()) == "trigger"){
				Trigger * trigger = new Trigger(curr);
				trigger_arr.push_back(trigger);
			}else if (string(curr->name()) == "border"){
				Border * newborder = new Border();
				string bdname, direction;
				// cout << "Border Construction - ";

				for(xml_node<> * b = curr->first_node(); b; b = b->next_sibling()) {
					if (string(b->name()) == "name") {
						bdname = b -> value();
						// cout << bdname << " - ";
					}else if (string(b->name()) == "direction") {
						direction = b -> value();
						// cout << direction << " " << endl;

						if(direction == "north")	{direction = "n";}
						if(direction == "south")	{direction = "s";}
						if(direction == "west")		{direction = "w";}
						if(direction == "east")		{direction = "e";}
					}
				}
				
				newborder -> direction = direction;
				newborder -> name = bdname;
				border_arr.push_back(newborder);
			}
		}
	}

	bool has_item(string obj) {
		for (int i = 0; i < item_arr.size(); i++){
			if (item_arr[i] == obj){
				return true;
			}
		}
		
		return false;
	}

	bool has_creature(string obj) {
		for (int i = 0; i < creature_arr.size(); i++){
			if (creature_arr[i] == obj){
				return true;
			}
		}
		
		return false;
	}

	bool has_container(string obj) {
		for (int i = 0; i < container_arr.size(); i++){
			if (container_arr[i] == obj){
				return true;
			}
		}
		
		return false;
	}
    /*Remove an element from border_arr.
     Source: https://stackoverflow.com/questions/991335/how-to-erase-delete-pointers-to-objects-stored-in-a-vector*/
    void removeBorder(string name) {
        vector<Border*>::iterator b;
        for (b = border_arr.begin(); b != border_arr.end();) {
            if ((*b)->name == name) {
                delete *b;
                b = border_arr.erase(b);
            } else {
                b++;
            }
        }
    }
};

#endif /* ROOM_H_ */
