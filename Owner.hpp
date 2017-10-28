//
//  Owner1.hpp
//  Zork
//
//  Created by Geoff on 10/28/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef Owner_hpp
#define Owner_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Owner {
public:
    Owner() {
        object = "";
        owner = "";
        has = false;
    }
    ~Owner() {}
    string object;
    bool has;
    string owner;
} Owner;


#endif /* Owner_hpp */
