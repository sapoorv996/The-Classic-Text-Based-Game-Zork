//
//  Trigger.hpp
//  Zork
//
//  Created by Geoff on 10/16/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef Trigger_hpp
#define Trigger_hpp

#include <stdio.h>
#include <vector>
#include "rapidxml.hpp"

using namespace std;

class Trigger {
public:
    Trigger(rapidxml::xml_node<>*);
    ~Trigger();
private:
    vector<Condition*> conditions;
    string type;
};

#endif /* Trigger_hpp */
