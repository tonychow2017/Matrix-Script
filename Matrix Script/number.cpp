//
//  number.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <string>
#include <sstream>
#include <cmath>
#include "number.hpp"

number::number(double v): value(v) {
}

number::number(const number& n): value(n.get_value()) {
}

number::~number() = default;

double number::get_value() const {
    return value;
}

std::string number::get_string_representation() const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

entry* number::clone() const {
    return new number(this->get_value());
}
