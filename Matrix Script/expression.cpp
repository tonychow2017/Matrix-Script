//
//  expression.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <sstream>
#include <vector>
#include "expression.hpp"

std::string expression::get_string_representation() const {
    std::ostringstream oss;
    oss << "e(";
    for (auto i=0; i<tokens.size(); i++) {
        oss << tokens[i].get_string_representation();
        if (i != tokens.size()-1) {
            oss << ",";
        }
    }
    oss << ")";
    return oss.str();
}

entry* expression::clone() const {
    std::vector<token> new_tokens(tokens);
    expression* e = new expression(new_tokens);
    return e;
}

expression::~expression() = default;

expression::expression(std::vector<token> t): tokens(t) {
}

const std::vector<token>& expression::get_tokens() const {
    return tokens;
}
