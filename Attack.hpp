#ifndef ATTACK_H_
#define ATTACK_H_

#include <string>
#include <vector>
#include <list>

#include "rapidxml.hpp"
#include "Condition.hpp"

using namespace rapidxml;
using namespace std;

class Attack {
public:
    Condition* condition;
    string print;
    vector<string> action;
    
    bool has_condition;
    bool has_print;
    bool has_action;
    
    Attack(xml_node<> * xnode) {setupNode(xnode);}
    ~Attack();

    void setupNode(xml_node<> * xnode) {
        for (xml_node<> * curr = xnode->first_node(); curr; curr = curr->next_sibling()){
            if (string(curr->name()) == "condition") {
                has_condition = true;
                for (xml_node<> * curr2 = xnode->first_node(); curr2; curr2 = curr->next_sibling()){
                    if (string(curr2->name()) == "status") {
                        condition->status = curr2->value();
                    } else if (string(curr2->name()) == "object") {
                        condition->object = curr2->value();
                    }
                }
            } else if (string(curr->name()) == "print") {
                has_print = true;
                print = curr->value();
            } else if (string(curr->name()) == "action") {
                has_action = true;
                action.push_back(curr->value());
            }
        }
    }
};

#endif /* CREATURE_H_ */

