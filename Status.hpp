//
//  Status.hpp
//  Zork
//
//  Created by Geoff on 10/28/17.
//  Copyright © 2017 Geoff. All rights reserved.
//

#ifndef Status_hpp
#define Status_hpp

#include <stdio.h>
#include <string>

using namespace std;

class Status {
public:
    Status() {
        object  = "";
        status = "";
    }
    ~Status() {}
    string object;
    string status;
};


#endif /* Status_hpp */
