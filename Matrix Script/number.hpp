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
#include "matrix.hpp"

class number: public entry {
public:
    number(double value);
    number(const number& n);
    ~number();
    number& operator=(number n);
    bool operator<(const number& n) const;
    double get_value() const;
    matrix as_matrix() const;
    std::string get_string_representation() const override;
    entry* clone() const override;
    bool is_zero() const;
    
private:
    double value;
};

#endif /* number_hpp */
