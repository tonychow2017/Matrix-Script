//
//  token.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef token_hpp
#define token_hpp

#include <string>
#include <algorithm>
#include "matrix.hpp"
#include "number.hpp"

class token {
public:
    static const int NUMBER = 0;
    static const int FUNCTION = 1;
    static const int SYMBOL = 2;
    static const int MATRIX = 3;
    token(const token& tok);
    token(number number);
    token(std::string function);
    token(char symbol);
    token(matrix matrix);
    ~token();
    std::string get_string_representation() const;
    int get_type() const;
    bool is_matrix_symbol() const;
    void* get_content() const;
    token as_matrix_token() const;
    token operator=(token rhs);
    
private:
    int type;
    void* content;
};

#endif /* token_hpp */
