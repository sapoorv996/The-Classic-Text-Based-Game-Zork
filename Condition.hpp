//
//  Condition.hpp
//  Zork
//
//  Created by Geoff on 10/16/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef Condition_hpp
#define Condition_hpp

#include <stdio.h>
#include <string>
#include "Item.hpp"

using namespace std;

class Condition {
    bool has;
    string object;
    Item owner;
};

#endif /* Condition_hpp */
