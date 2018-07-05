//
//  error.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 19/4/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef error_hpp
#define error_hpp

#include <string>
#include <unordered_map>

class error {
private:
    int type;
    std::string message;
    static std::unordered_map<int,std::string> meaning;
    
public:
    static const int NO_ERROR;
    static const int ERROR_UNKNOWN_CHAR;
    static const int ERROR_EXTRA_DOTS;
    static const int ERROR_ENDS_WITH_DOTS;
    static const int ERROR_UNCLOSED_MATRIX;
    static const int ERROR_MISMATCHED_BRACKET;
    static const int ERROR_WRONG_ARG_COUNT;
    static const int ERROR_DIM_MISMATCH;
    static const int ERROR_DIV_BY_ZERO;
    static const int ERROR_NON_SQUARE_MATRIX;
    static const int ERROR_SINGULAR_MATRIX;
    static const int ERROR_ENTRY_NOT_ALL_NUMBER;
    static const int ERROR_TRIGO;
    static const int ERROR_LOGARITHM;
    static const int ERROR_POWER;
    static const int ERROR_NOT_NUMBER;
    static const int ERROR_NOT_NATURAL_NUMBER;
    static const int ERROR_INTERNAL;
    static const int ERROR_UNKNOWN_FUNC;
    static const int ERROR_UNKNOWN_VAR;
    static const int ERROR_INVALID_VAR_NAME;
    static const int ERROR_EMPTY_MATRIX;
    static const int ERROR_NOTHING_ON_RHS;
    static const int ERROR_NOT_ROW_VECTOR;
    static const int ERROR_OUT_OF_BOUNDARY;
    error();
    error(int);
    error(int,std::string);
    int get_type();
    std::string get_message();
    static std::string get_error_meaning(int);
};

#endif /* error_hpp */
