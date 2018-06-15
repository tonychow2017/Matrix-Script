//
//  main_calculator.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef main_calculator_hpp
#define main_calculator_hpp

#include <string>
#include <vector>
#include <stack>
#include <unordered_map>
#include <algorithm>
#include "token.hpp"
#include "matrix.hpp"
#include "expression.hpp"
#include "error.hpp"

//extern int ERROR_EXTRA_DOTS, ERROR_ENDS_WITH_DOTS, ERROR_UNKNOWN_CHAR;
extern std::unordered_map<std::string,matrix> variables;

bool is_matrix_relevant_symbol(char c);
expression merge_matrix(expression exp, bool& has_error, error& e);
void get_answer(std::string input, matrix& result, bool& has_error, error& e);
std::string calculate(std::string input);

#endif /* main_calculator_hpp */
