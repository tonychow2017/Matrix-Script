//
//  number.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef number_hpp
#define number_hpp

#include "entry.hpp"

class number: public entry {
public:
    number(double value);
    number(const number& n);
    ~number();
    double get_value() const;
    std::string get_string_representation() const override;
    entry* clone() const override;
    
private:
    double value;
};

#endif /* number_hpp */
