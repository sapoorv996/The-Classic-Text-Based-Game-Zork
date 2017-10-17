//
//  Item.hpp
//  Zork
//
//  Created by Geoff on 10/16/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef Item_hpp
#define Item_hpp

#include <stdio.h>
#include <string>
#include <vector>
#include "rapidxml.hpp"

using namespace std;

class Item {
public:
    Item(rapidxml::xml_node<>*);
    virtual ~Item();
private:
    string name;
    string description;
    string writing;
    vector<string> turnon;
    void setUpItem(rapidxml::xml_node<>*);
    void setUpTurnOn(rapidxml::xml_node<>*);
};

#endif /* Item_hpp */
