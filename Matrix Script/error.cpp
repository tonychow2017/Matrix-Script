//
//  error.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 19/4/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <unordered_map>
#include <utility>
#include <stdexcept>
#include "error.hpp"

const int error::NO_ERROR = 0;
const int error::ERROR_UNKNOWN_CHAR = 1;
const int error::ERROR_EXTRA_DOTS = 2;
const int error::ERROR_ENDS_WITH_DOTS = 3;
const int error::ERROR_UNCLOSED_MATRIX = 4;
const int error::ERROR_MISMATCHED_BRACKET = 5;
const int error::ERROR_WRONG_ARG_COUNT = 6;
const int error::ERROR_DIM_MISMATCH = 7;
const int error::ERROR_DIV_BY_ZERO = 8;
const int error::ERROR_NON_SQUARE_MATRIX = 9;
const int error::ERROR_SINGULAR_MATRIX = 10;
const int error::ERROR_ENTRY_NOT_ALL_NUMBER = 12;
const int error::ERROR_TRIGO = 13;
const int error::ERROR_LOGARITHM = 14;
const int error::ERROR_POWER = 15;
const int error::ERROR_NOT_NUMBER = 16;
const int error::ERROR_NOT_NATURAL_NUMBER = 17;
const int error::ERROR_UNKNOWN_VAR = 18;
const int error::ERROR_INVALID_VAR_NAME = 19;
const int error::ERROR_EMPTY_MATRIX = 20;
const int error::ERROR_NOTHING_ON_RHS = 21;
const int error::ERROR_NOT_ROW_VECTOR = 22;
const int error::ERROR_OUT_OF_BOUNDARY = 23;
const int error::ERROR_INTERNAL = 500;
const int error::ERROR_UNKNOWN_FUNC = 11;
std::unordered_map<int,std::string> error::meaning;


error::error(): type(error::NO_ERROR), message("") {
}

error::error(int t, std::string mess): type(t), message(mess) {
}

error::error(int t): type(t), message("") {
}

int error::get_type() {
    return type;
}

std::string error::get_message() {
    if (message != std::string("")) {
        return std::string("Error: ") + get_error_meaning(type) + ": " + message + ".";
    } else {
        return std::string("Error: ") + get_error_meaning(type) + ".";
    }
}

std::string error::get_error_meaning(int e) {
    //std::unordered_map<int,std::string> meaning;
    if (meaning.empty()) {
        meaning.insert(std::make_pair(ERROR_POWER,"Power function"));
        meaning.insert(std::make_pair(ERROR_UNKNOWN_CHAR,"Unknown character"));
        meaning.insert(std::make_pair(ERROR_TRIGO,"Trigonometric function"));
        meaning.insert(std::make_pair(ERROR_INTERNAL,"Internal"));
        meaning.insert(std::make_pair(ERROR_EXTRA_DOTS,"Extra dots"));
        meaning.insert(std::make_pair(ERROR_ENDS_WITH_DOTS,"Ends with dot"));
        meaning.insert(std::make_pair(ERROR_UNCLOSED_MATRIX,"Unclosed matrix"));
        meaning.insert(std::make_pair(ERROR_MISMATCHED_BRACKET,"Mismatched bracket"));
        meaning.insert(std::make_pair(ERROR_WRONG_ARG_COUNT,"Wrong number of argument(s)"));
        meaning.insert(std::make_pair(ERROR_DIM_MISMATCH,"Dimension mismatch"));
        meaning.insert(std::make_pair(ERROR_DIV_BY_ZERO,"Division by zero"));
        meaning.insert(std::make_pair(ERROR_NON_SQUARE_MATRIX,"Non-square matrix"));
        meaning.insert(std::make_pair(ERROR_SINGULAR_MATRIX,"Singular matrix"));
        meaning.insert(std::make_pair(ERROR_ENTRY_NOT_ALL_NUMBER,"Entries not all number"));
        //const int error::ERROR_TRIGO = 13;
        meaning.insert(std::make_pair(ERROR_LOGARITHM,"Logarithm"));
        //const int error::ERROR_POWER = 15;
        meaning.insert(std::make_pair(ERROR_NOT_NUMBER,"Not a number"));
        meaning.insert(std::make_pair(ERROR_NOT_NATURAL_NUMBER, "Not a natural number"));
        //const int error::ERROR_INTERNAL = 500;
        meaning.insert(std::make_pair(ERROR_UNKNOWN_FUNC,"Unknown function"));
        meaning.insert(std::make_pair(ERROR_UNKNOWN_VAR,"Unknown variable"));
        meaning.insert(std::make_pair(ERROR_INVALID_VAR_NAME,"Invalid variable name"));
        meaning.insert(std::make_pair(ERROR_EMPTY_MATRIX,"Empty matrix"));
        meaning.insert(std::make_pair(ERROR_NOTHING_ON_RHS,"Assign nothing to variable"));
        meaning.insert(std::make_pair(ERROR_NOT_ROW_VECTOR,"Require a row vector"));
        meaning.insert(std::make_pair(ERROR_OUT_OF_BOUNDARY,"Out of matrix boundary"));
    }
    try {
        std::string result = meaning.at(e);
        return result;
    } catch (const std::out_of_range& e) {
        return "Unknown";
    }
}

