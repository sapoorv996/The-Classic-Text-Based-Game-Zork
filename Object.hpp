//
//  Object.hpp
//  Zork
//
//  Created by Geoff on 10/16/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef Object_hpp
#define Object_hpp

#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class Object {
private:
    string status;
    vector<Trigger*> triggers;
public:
    Object();
    virtual ~Object();
};

#endif /* Object_hpp */
