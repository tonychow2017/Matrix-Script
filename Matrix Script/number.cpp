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

number& number::operator=(number n) {
    this->value = n.get_value();
    return *this;
}

bool number::operator<(const number& n) {
    return this->value < n.get_value();
}

double number::get_value() const {
    return value;
}

std::string number::get_string_representation() const {
    std::ostringstream oss;
    oss << value;
    return oss.str();
}

matrix number::as_matrix() const {
    matrix m(1,1);
    m.set(0,0,*this);
    return m;
}

entry* number::clone() const {
    return new number(this->get_value());
}

bool number::is_zero() const {
    return this->get_value() == 0;
}
