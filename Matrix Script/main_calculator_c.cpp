//
//  main_calculator_c.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <cstdio>
#include <cstring>
#include "main_calculator_c.hpp"
#include "main_calculator.hpp"

void calculate_c(char** result, const char* input) {
    std::string input_string(input);
    std::string result_string = calculate(input_string);
    const char* result_c_string = result_string.c_str();
    char* returning = (char*)malloc(result_string.size()+1);
    strcpy(returning,result_c_string);
    *result = returning;
}

void reduceMemoryUsage() {
    reduceMemoryUsageCpp();
}

