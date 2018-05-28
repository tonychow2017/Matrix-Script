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

class error {
private:
    int type;
    std::string message;
    
public:
    static const int NO_ERROR = 0;
    static const int ERROR_UNKNOWN_CHAR = 1;
    static const int ERROR_EXTRA_DOTS = 2;
    static const int ERROR_ENDS_WITH_DOTS = 3;
    static const int ERROR_UNCLOSED_MATRIX = 4;
    static const int ERROR_MISMATCHED_BRACKET = 5;
    static const int ERROR_WRONG_ARG_COUNT = 6;
    static const int ERROR_DIM_MISMATCH = 7;
    static const int ERROR_DIV_BY_ZERO = 8;
    static const int ERROR_NON_SQUARE_MATRIX = 9;
    static const int ERROR_SINGULAR_MATRIX = 10;
    static const int ERROR_INTERNAL = 500;
    static const int ERROR_UNKNOWN_FUNC = 10;
    error();
    error(int,std::string);
    int get_type();
    std::string get_message();
};

#endif /* error_hpp */
