//
//  entry.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef entry_hpp
#define entry_hpp

#include <string>
#include <stdio.h>

class entry {
public:
    virtual std::string get_string_representation() const = 0;
    virtual entry* clone() const = 0;
    virtual ~entry();
};

#endif /* entry_hpp */
