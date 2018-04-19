//
//  token.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include "token.hpp"
#include "main_calculator.hpp"

/*const int token::NUMBER = 0;
const int token::FUNCTION = 1;
const int token::SYMBOL = 2;
const int token::MATRIX = 3;*/

token::token(const token& tok) {
    type = tok.get_type();
    switch (type) {
        case NUMBER:
            content = new number(*(number*)(tok.content));
            break;
        case FUNCTION:
            content = new std::string(*(std::string*)(tok.content));
            break;
        case SYMBOL:
            content = new char;
            *(char*)content = *(char*)(tok.content);
            break;
        case MATRIX:
            content = new matrix(*(matrix*)(tok.content));
            break;
        default:
            break;
    }
}

token::token(number n) {
    type = NUMBER;
    content = new number(n);
}

token::token(std::string function_name) {
    type = FUNCTION;
    content = new std::string(function_name);
}

token::token(char symbol) {
    type = SYMBOL;
    content = new char;
    *(char*)content = symbol;
}

token::token(matrix mat) {
    type = MATRIX;
    content = new matrix(mat);
}

token::~token() {
    switch (type) {
        case NUMBER:
            delete (number*)content;
            break;
        case FUNCTION:
            delete (std::string*)content;
            break;
        case SYMBOL:
            delete (char*)content;
            break;
        case MATRIX:
            delete (matrix*)content;
            break;
        default:
            break;
    }
}

int token::get_type() const {
    return type;
}

std::string token::get_string_representation() const {
    switch (type) {
        case NUMBER:
            return ((number*)content)->get_string_representation();
        case FUNCTION:
            return *(std::string*)content;
        case SYMBOL:
        {
            std::string character(1,*(char*)content);
            return character;
        }
        case MATRIX:
            return ((matrix*)content)->get_string_representation();
        default:
            return "UNKNOWN";
    }
}

bool token::is_matrix_symbol() const {
    if (type != SYMBOL) {
        return false;
    } else {
        return is_matrix_relevant_symbol(*(char*)content);
    }
}

void* token::get_content() const {
    return content;
}

token token::as_matrix_token() const {
    if (type == NUMBER) {
        matrix m(1,1);
        m.set(0,0,*(number*)content);
        token new_token(m);
        return new_token;
    } else {
        return *this;
    }
}

token token::operator=(token rhs) {
    std::swap(this->type,rhs.type);
    void* tmp = rhs.content;
    this->content = tmp;
    rhs.content = nullptr; //cannot use swap?
    return *this;
}

