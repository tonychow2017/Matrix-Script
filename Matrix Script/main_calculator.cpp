//
//  main_calculator.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <string>
#include <sstream>
#include <algorithm>
#include <utility>
#include <regex>
#include <stdexcept>
#include "main_calculator.hpp"
#include <iostream>
#include "entry.hpp"
#include "expression.hpp"
#include "function.hpp"
#include "error.hpp"

std::unordered_map<std::string,matrix> variables;
std::unordered_map<std::string,size_t> argcount;

void set_variable(std::string name, matrix& ent) {
    variables[name] = ent;
}

matrix* get_variable(std::string name) {
    try {
        matrix& result = variables.at(name);
        return &result;
    } catch (const std::out_of_range& e) {
        return nullptr;
    }
}

inline bool is_digit(char c) {
    return ('0' <= c) && (c <= '9');
}

bool is_number(std::string s) {
    if (s.empty()) {
        return false;
    } else if (s[s.size()-1] == '.') {
        return false;
    } else {
        bool already_has_dot = false;
        for (size_t i=0; i<s.size(); i++) {
            char c = s[i];
            if (c == '.') {
                if (already_has_dot) {
                    return false;
                } else {
                    already_has_dot = true;
                }
            } else if (!is_digit(c)) {
                return false;
            }
        }
        return true;
    }
}

inline bool is_letter(char c) {
    return (('A' <= c) && (c <= 'Z')) || (('a' <= c) && (c <= 'z'));
}

inline bool is_valid_function_identifier(char c) {
    return is_letter(c) || (c == '_');
}

inline bool is_valid_symbol(char c) {
    return std::string("+-*/^~!%()[].,;:$=#_\n").find(c) != std::string::npos;
}

bool is_matrix_relevant_symbol(char c) {
    return std::string("[],;").find(c) != std::string::npos;
}

inline bool is_bracket(char c) {
    return std::string("[]()").find(c) != std::string::npos;
}

inline bool is_open_bracket(char c) {
    return c == '[' || c == '(';
}

inline bool is_closed_bracket(char c) {
    return c == ']' || c == ')';
}

void process_line(std::string input, matrix& result, bool& has_error, error& e, size_t& error_line) {
    std::istringstream iss(input);
    std::string line;
    size_t lineno = 0;
    while (std::getline(iss,line)) {
        lineno++;
        line = preprocess(line,has_error,e);
        if (has_error) {
            error_line = lineno;
            return;
        }
        size_t pos = line.find("=");
        std::string var_name;
        if (std::string::npos != pos) {
            //found
            var_name = line.substr(0,pos);
            if (var_name.empty()) {
                has_error = true;
                e = error(error::ERROR_INVALID_VAR_NAME,"Empty");
                error_line = lineno;
                return;
            } else if (var_name == "$") {
                has_error = true;
                e = error(error::ERROR_INVALID_VAR_NAME,"$");
                error_line = lineno;
                return;
            } else if (var_name[0] == '$') {
                var_name = var_name.substr(1,var_name.size()-1);
            }
            line = line.substr(pos+1,line.size()-pos-1);
            if (line.empty()) {
                has_error = true;
                e = error(error::ERROR_NOTHING_ON_RHS);
                error_line = lineno;
                return;
            }
            for (size_t i=0; i<var_name.size(); i++) {
                if (!is_valid_function_identifier(var_name.at(i))) {
                    has_error = true;
                    e = error(error::ERROR_INVALID_VAR_NAME,std::string(1,var_name[i]));
                    error_line = lineno;
                    return;
                }
            }

        } /*else {
            var_name = "answer";
        }*/
        //matrix result(0,0);
        //bool has_error;
        //error e;
        get_answer(line,result,has_error,e);
        if (!has_error) {
            set_variable("answer",result);
            if (var_name != "answer") {
                set_variable(var_name,result);
            }
        } else {
            error_line = lineno;
            return;
        }
    }
}

std::string preprocess(std::string input, bool& has_error, error& e) {
    std::ostringstream oss;
    for (size_t i=0; i<input.size(); i++) {
        char c = input[i];
        if (is_letter(c) || is_digit(c) || is_valid_symbol(c)) {
            oss << c;
        } else if (c != ' ') {
            has_error = true;
            std::string char_string(1,c);
            e = error(error::ERROR_UNKNOWN_CHAR,char_string);
            return oss.str();
        }
    }
    has_error = false;
    std::string result = oss.str();
    result = std::regex_replace(result,std::regex("\\(-"),"(0-");
    result = std::regex_replace(result,std::regex("\\[-"),"[0-");
    result = std::regex_replace(result,std::regex("\\,-"),",0-");
    result = std::regex_replace(result,std::regex(";-"),";0-");
    result = std::regex_replace(result,std::regex("\\+-"),"-");
    result = std::regex_replace(result,std::regex("-\\+"),"-");
    result = std::regex_replace(result,std::regex("\\+\\+"),"(+");
    result = std::regex_replace(result,std::regex("--"),"+");
    result = std::regex_replace(result,std::regex("\\s"),"");
    result = std::regex_replace(result,std::regex("\\t"),"");
    if (result[0] == '-') {
        std::ostringstream oss;
        oss << "0" << result;
        result = oss.str();
    }
    debug_print(std::cout << "preprocessed: " << result << "\n";)
    return result;
}

expression tokenize(std::string input, bool& has_error, error& e) {
    std::vector<token> tokens;
    size_t input_size = input.size();
    for (size_t i=0; i<input_size; i++) {
        char c = input[i];
        debug_print(std::cout << "current char " << i << "/" << input_size << ": " << c << std::endl;)
        if (c == '$') {
            size_t index = i+1;
            while (is_valid_function_identifier(input[index])) {
                index++;
                if (index == input_size) {
                    break;
                }
            }
            std::string var_name = input.substr(i,index-i);
            token t(var_name);
            tokens.push_back(t);
            i = index-1;
            continue;
        } else if (is_letter(c)) {
            //find until function ends
            size_t index = i+1;
            while (is_valid_function_identifier(input[index])) {
                index++;
                if (index == input_size) {
                    break;
                }
            }
            //now input[i] is either not within the function name, or is out of bounds
            //so function name from i to index-1 inclusive
            std::string function_name = input.substr(i,index-i);
            token t(function_name);
            tokens.push_back(t);
            i = index-1;
            continue;
        } else if (is_digit(c)) {
            //find until number ends
            size_t index = i+1;
            bool already_has_dot = false;
            while (is_digit(input[index]) || (input[index] == '.')) {
                debug_print(std::cout << "index: " << index << " is digit\n";)
                if (input[index] == '.') {
                    if (already_has_dot) {
                        e = error(error::ERROR_EXTRA_DOTS,input.substr(i,index-i));
                        has_error = true;
                        return tokens;
                    } else {
                        already_has_dot = true;
                    }
                }
                index++;
                if (index == input.size()) {
                    break;
                }
            }
            //now substring from i to index-1 inclusive is a number
            if (input[index-1] == '.') {
                e = error(error::ERROR_ENDS_WITH_DOTS,input.substr(i,index-1-i));
                has_error = true;
                return tokens;
            } else {
                std::string digits = input.substr(i,index-i);
                std::ostringstream oss;
                if (digits[0] == '.') {
                    oss << "0";
                }
                oss << digits;
                std::istringstream iss(oss.str());
                double result;
                iss >> result;
                number n(result);
                debug_print(std::cout << "number is: " << result << "\n";)
                token t(n);
                tokens.push_back(t);
                i = index-1;
                continue;
            }
        } else if (c == '#') {
            return tokens;
        } else if (is_valid_symbol(c)) {
            debug_print(std::cout << "symbol\n";)
            token t(c);
            tokens.push_back(t);
        } else {
            e = error(error::ERROR_UNKNOWN_CHAR,std::string(1,c));
            has_error = true;
            return tokens;
        }
    }
    has_error = false;
    expression result(tokens);
    return result;
}

matrix build_matrix(std::vector<token> tokens, bool& has_error, error& e) {
    debug_print(std::cout << "now build matrix: ";)
    debug_print(for (auto& i: tokens) {
        std::cout << i.get_string_representation() << " ";
    })
    debug_print(std::cout << "\n";)
    std::vector<std::vector<expression>> rows;
    std::vector<expression> row;
    std::vector<token> exp;
    int max_col = 0;
    for (auto i=0; i<=tokens.size(); i++) {
        //std::vector<token> exp;
        debug_print(std::cout << "current token is: " << tokens[i].get_string_representation() << "\n";)
        while (i != tokens.size() && tokens[i].get_string_representation() != "," && tokens[i].get_string_representation() != ";" && tokens[i].get_string_representation() != "(" && tokens[i].get_string_representation() != "[") {
            exp.push_back(tokens[i]);
            i++;
        }
        if (i != tokens.size() && (tokens[i].get_string_representation() == "(" || tokens[i].get_string_representation() == "[")) {
            //exp.push_back(tokens[i]);
            std::stack<std::string> brackets;
            //brackets.push(tokens[i].get_string_representation());
            while (i != tokens.size()) {
                //i++;
                if (i == tokens.size()) {
                    has_error = true;
                    e = error(brackets.top() == "("?error::ERROR_MISMATCHED_BRACKET:error::ERROR_UNCLOSED_MATRIX);
                    return matrix(0,0);
                    //break;
                } else {
                    exp.push_back(tokens[i]);
                    const std::string& rep = tokens[i].get_string_representation();
                    debug_print(std::cout << "build matrix: sub-matrix or exp: rep: " << rep << "; exp contains: " << exp.size() << std::endl;)
                    if (is_closed_bracket(rep[0])) {
                        brackets.pop();
                    } else if (is_open_bracket(rep[0])) {
                        brackets.push(rep);
                    }
                    if (brackets.empty()) {
                        debug_print(std::cout << "empty\n";)
                        break;
                    }
                    i++;
                }
            }
            /*if (i == tokens.size()) {
                has_error = true;
                e = error(brackets.top() == "("?error::ERROR_MISMATCHED_BRACKET:error::ERROR_UNCLOSED_MATRIX);
                return matrix(0,0);
            }*/
            debug_print(std::cout << "while-end: exp contains: " << exp.size() << "\n";)
        }
        debug_print(std::cout << "leaving if: exp size: " << exp.size() << "\n";)
        if (i == tokens.size() || tokens[i].get_string_representation() == ";") {
            //end row
            debug_print(std::cout << "end row: exp contains: " << exp.size() << "\n";)
            expression e(exp);
            row.push_back(e);
            max_col = std::max((int)(row.size()),max_col);
            rows.push_back(row);
            //reset row
            row = std::vector<expression>(); //.clear();
            exp = std::vector<token>(); //.clear();
        } else if (tokens[i].get_string_representation() == ",") {
            debug_print(std::cout << ",\n";)
            expression e(exp);
            row.push_back(e);
            exp = std::vector<token>(); //.clear();
        }
        debug_print(std::cout << "end of for loop body: i: " << i << ", size: " << tokens.size() << "\n";)
    }
    debug_print(std::cout << "creating matrix of size " << rows.size() << " " << max_col << std::endl;)
    matrix m(rows.size(),max_col);
    for (size_t i=0; i<rows.size(); i++) {
        for (size_t j=0; j<rows[i].size(); j++) {
            expression matrix_exp = merge_matrix(rows[i][j],has_error,e);
            if (has_error) {
                return m;
            }
            m.set(i,j,matrix_exp);
        }
        if (rows[i].size() != max_col) {
            for (size_t k=rows[i].size(); k<max_col; k++) {
                number n(0);
                token t(n);
                std::vector<token> tokens;
                tokens.push_back(t);
                expression token_exp(tokens);
                m.set(i,k,token_exp);
            }
        }
    }
    return m;
}

expression merge_matrix(expression exp, bool& has_error, error& e) {
    std::vector<token> tokens = exp.get_tokens();
    //when encounter [
    //find until corresponding ]
    //then recursively do on each entry
    std::vector<token> result;
    for (auto i=0; i<tokens.size(); i++) {
        token& tok = tokens[i];
        if (tok.get_type() == token::SYMBOL && tok.get_string_representation() == "[") {
            debug_print(std::cout << "token is open\n";)
            size_t open_count = 0;
            auto index = i;
            index++;
            while (true) {
                if (index == tokens.size()) {
                    debug_print(std::cout << "does not end?\n";)
                    has_error = true;
                    e = error(error::ERROR_UNCLOSED_MATRIX,"");
                    return expression(result); //unclosed?
                } else if (!tokens[index].is_matrix_symbol()) {
                    debug_print(std::cout << "index " << index << " is not matrix symbol\n";)
                    index++;
                } else if (tokens[index].get_string_representation() == "[") {
                    debug_print(std::cout << "index " << index << " is [\n";)
                    open_count++;
                    index++;
                } else if (tokens[index].get_string_representation() == "]") {
                    if (open_count == 0) {
                        //now [i,index] is a matrix
                        std::vector<token> sub(tokens.begin()+i+1,tokens.begin()+index);
                        /*std::cout << "dealing with vector:\n";
                        for (token& tok: sub) {
                            std::cout << tok.get_string_representation() << std::endl;
                        }*/
                        matrix mat = build_matrix(sub, has_error, e);
                        if (has_error) {
                            return expression(result);
                        }
                        token mat_token(mat);
                        result.push_back(mat_token);
                        i = index;
                        break;
                    } else {
                        open_count--;
                        index++;
                    }
                } else {
                    index++;
                }
            }
        } else {
            debug_print(std::cout << "token " << tok.get_string_representation() << " is not [" << std::endl;)
            result.push_back(tok);
        }
    }
    has_error = false;
    expression resulting_exp(result);
    return resulting_exp;
}

int get_precedence(const token& op) {
    switch (op.get_string_representation()[0]) {
        case '!':
        case '%':
            return 5;
        case '~':
            return 4;
        case '^':
            return 3;
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

bool is_left_associative(const token& op) {
    return op.get_string_representation() != "^";
}

expression shunting_yard(const expression& exp, bool& has_error, error& e) {
    debug_print(std::cout << "now do shunting yard: " << exp.get_string_representation() << std::endl;)
    std::vector<token> resulting_token;
    std::stack<token> operator_stack;
    /*
     algorithm:
     source: Wikipedia entry: Shunting-yard algorithm
     */
    std::vector<token> original_tokens = exp.get_tokens();
    for (size_t next_element = 0; next_element < original_tokens.size(); next_element++) {
        token next = original_tokens[next_element];
        debug_print(std::cout << next.get_type() << ", " << next.get_string_representation() << std::endl;)
        switch (next.get_type()) {
            case token::NUMBER:
                resulting_token.push_back(next);
                debug_print(std::cout << "insert " << next.get_string_representation() << " to result\n";)
                break;
                
            case token::FUNCTION:
                operator_stack.push(next);
                break;
                
            case token::SYMBOL:
                if (next.get_string_representation() == "(") {
                    operator_stack.push(next);
                } else if (next.get_string_representation() == ")") {
                    while ((!operator_stack.empty()) && operator_stack.top().get_string_representation() != "(") {
                        token t = operator_stack.top();
                        operator_stack.pop();
                        resulting_token.push_back(t);
                        debug_print(std::cout << "insert " << t.get_string_representation() << " to result\n";)
                    }
                    if (operator_stack.empty()) {
                        //mismatched bracket?
                        has_error = true;
                        e = error(error::ERROR_MISMATCHED_BRACKET);
                        expression result(resulting_token);
                        return result;
                    } else {
                        operator_stack.pop();
                    }
                //} else if (next.get_string_representation() == ",") {
                    //part of expression ends
                } else {
                    while ((!operator_stack.empty()) && ((operator_stack.top().get_type() == token::FUNCTION) || (get_precedence(operator_stack.top()) > get_precedence(next)) || (get_precedence(operator_stack.top()) == get_precedence(next) && is_left_associative(operator_stack.top()))) && (operator_stack.top().get_string_representation() != "(")) {
                        token top_token = operator_stack.top();
                        operator_stack.pop();
                        resulting_token.push_back(top_token);
                        debug_print(std::cout << "insert " << top_token.get_string_representation() << " to result\n";)
                    }
                    if (next.get_string_representation() != ",") {
                        operator_stack.push(next);
                    }
                }
                break;
                
            case token::MATRIX:
                //call shunting yard for each entry
                //then treat it as a number
                {
                    matrix* m = (matrix*)(next.get_content());
                    for (size_t i=0; i<m->row_count(); i++) {
                        for (size_t j=0; j<m->column_count(); j++) {
                            debug_print(std::cout << "getting matrix " << i << " " << j << "\n";)
                            entry* ent = m->get(i,j);
                            expression* exp;
                            debug_print(std::cout << "trying dynamic cast\n";)
                            if ((exp = dynamic_cast<expression*>(ent)) != nullptr) {
                                debug_print(std::cout << "--do shunting yard on " << exp->get_string_representation() << "\n";)
                                expression done = shunting_yard(*exp,has_error,e);
                                if (has_error) {
                                    return done;
                                }
                                m->set(i,j,done);
                            }
                        }
                    }
                    resulting_token.push_back(next);
                    break;
                }
                        
            default:
                break;
        }
    }
    while (!operator_stack.empty()) {
        token t = operator_stack.top();
        std::string t_str = t.get_string_representation();
        if (t_str == "(" || t_str == ")") {
            has_error = true;
            e = error(error::ERROR_MISMATCHED_BRACKET,"");
            expression exp(resulting_token);
            return exp;
        } else {
            operator_stack.pop();
            resulting_token.push_back(t);
        }
    }
    expression result(resulting_token);
    has_error = false;
    return result;
}

size_t get_function_argument_count(const std::string& name) {
    if (argcount.empty()) {
        argcount.emplace("+",2);
        argcount.emplace("-",2);
        argcount.emplace("*",2);
        argcount.emplace("/",2);
        argcount.emplace("^",2);
        argcount.emplace("~",2);
        argcount.emplace("!",1);
        argcount.emplace("%",1);
        argcount.emplace("log",2);
        argcount.emplace("inv",1);
        argcount.emplace("det",1);
        argcount.emplace("rref",1);
        argcount.emplace("abs",1);
        argcount.emplace("ceil",1);
        argcount.emplace("floor",1);
        argcount.emplace("round",1);
        argcount.emplace("exp",1);
        argcount.emplace("ln",1);
        argcount.emplace("sqrt",1);
        argcount.emplace("sin",1);
        argcount.emplace("cos",1);
        argcount.emplace("tan",1);
        argcount.emplace("asin",1);
        argcount.emplace("acos",1);
        argcount.emplace("atan",1);
        argcount.emplace("csc",1);
        argcount.emplace("sec",1);
        argcount.emplace("cot",1);
        argcount.emplace("acsc",1);
        argcount.emplace("asec",1);
        argcount.emplace("acot",1);
        argcount.emplace("size",1);
        argcount.emplace("rowcount",1);
        argcount.emplace("colcount",1);
        argcount.emplace("columncount",1);
        argcount.emplace("fact",1);
        argcount.emplace("factorial",1);
        argcount.emplace("flatten",1);
        argcount.emplace("sum",1);
        argcount.emplace("prod",1);
        argcount.emplace("product",1);
        argcount.emplace("transpose",1);
        argcount.emplace("t",1);
        argcount.emplace("max",1);
        argcount.emplace("min",1);
        argcount.emplace("maxmin",1);
        argcount.emplace("range",2);
        argcount.emplace("sort",1);
        argcount.emplace("one",2);
        argcount.emplace("zero",2);
        argcount.emplace("id",1);
        argcount.emplace("eye",1);
        argcount.emplace("get",3);
        argcount.emplace("resize",3);
        argcount.emplace("replace",4);
        argcount.emplace("append",2);
        argcount.emplace("union",2);
        argcount.emplace("intersection",2);
        argcount.emplace("sym_diff",2);
        argcount.emplace("unique",1);
        argcount.emplace("remove",2);
        argcount.emplace("rm",2);
        argcount.emplace("median",1);
        argcount.emplace("reverse",1);
        argcount.emplace("rev",1);
        argcount.emplace("tr",1);
        argcount.emplace("trace",1);
        argcount.emplace("mat_pow",2);
    }
    auto it = argcount.find(name);
    if (it != argcount.end()) {
        return argcount[name];
    } else {
        return 0;
    }
    /*if (name == "+" || name == "-" || name == "*" || name == "/" || name == "^" || name == "log") {
        return 2;
    } else if ((!name.empty()) && (name.at(0) == '$' || name == "pi")) {
        return 0;
    } else {
        return 1;
    }*/
}

matrix evaluate_function(const std::string& name, const std::vector<token>& argv, bool& has_error, error& e) {
    if ((!name.empty()) && (name.at(0) == '$')) {
        matrix* got = get_variable(name.substr(1,name.size()-1));
        if (got != nullptr) {
            return *got;
        } else {
            has_error = true;
            e = error(error::ERROR_UNKNOWN_VAR,name);
            return matrix(0,0);
        }
    } else if (name == "pi") {
        return pi().as_matrix();
    } else if (name == "goldenratio" || name == "golden_ratio") {
        return number((1+std::sqrt(5))/2).as_matrix();
    } else if (name == "gravitational_const") {
        return number(6.67408e-11).as_matrix();
    } else if (name == "gas_const") {
        return number(8.3144598).as_matrix();
    } else if (get_function_argument_count(name) >= 1) {
        token t1 = argv[0];
        matrix m1 = *(matrix*)(t1.get_content());
        if (name == "inv") {
            return matrix_inv(m1,has_error,e);
        } else if (name == "det") {
            return matrix_det(m1,has_error,e).as_matrix();
        } else if (name == "rref") {
            return matrix_rref(m1,has_error,e);
        } else if (name == "abs") {
            return matrix_func(m1,&number_abs);
        } else if (name == "ceil") {
            return matrix_func(m1,&number_ceil);
        } else if (name == "floor") {
            return matrix_func(m1,&number_floor);
        } else if (name == "round") {
            return matrix_func(m1,&number_round);
        } else if (name == "exp") {
            return matrix_func_error(m1,&number_exp,has_error,e);
        } else if (name == "ln") {
            return matrix_func_error(m1,&number_ln,has_error,e);
        } else if (name == "sqrt") {
            return matrix_func_error(m1,&number_sqrt,has_error,e);
        } else if (name == "sin") {
            return matrix_func(m1,&number_sin);
        } else if (name == "cos") {
            return matrix_func(m1,&number_cos);
        } else if (name == "tan") {
            return matrix_func_error(m1,&number_tan,has_error,e);
        } else if (name == "asin") {
            return matrix_func_error(m1,&number_asin,has_error,e);
        } else if (name == "acos") {
            return matrix_func_error(m1,&number_acos,has_error,e);
        } else if (name == "atan") {
            return matrix_func(m1,&number_atan);
        } else if (name == "csc") {
            return matrix_func_error(m1,&number_csc,has_error,e);
        } else if (name == "sec") {
            return matrix_func_error(m1,&number_sec,has_error,e);
        } else if (name == "cot") {
            return matrix_func_error(m1,&number_cot,has_error,e);
        } else if (name == "size") {
            return matrix_size(m1);
        } else if (name == "rowcount") {
            return matrix_row(m1).as_matrix();
        } else if (name == "colcount" || name == "columncount") {
            return matrix_col(m1).as_matrix();
        } else if (name == "fact" || name == "factorial" || name == "!") {
            return matrix_func_error(m1,&number_factorial,has_error,e);
        } else if (name == "sum") {
            return matrix_sum(m1,has_error,e);
        } else if (name == "prod" || name == "product") {
            return matrix_product(m1,has_error,e);
        } else if (name == "median") {
            return matrix_median(m1,has_error,e);
        } else if (name == "flatten") {
            return matrix_flatten(m1);
        } else if (name == "transpose" || name == "t") {
            return matrix_transpose(m1);
        } else if (name == "unique") {
            return matrix_unique(m1);
        } else if (name == "max") {
            return matrix_max(m1,has_error,e).as_matrix();
        } else if (name == "min") {
            return matrix_min(m1,has_error,e).as_matrix();
        } else if (name == "maxmin") {
            return matrix_maxmin(m1,has_error,e);
        } else if (name == "sort") {
            return matrix_sort(m1,has_error,e);
        } else if (name == "reverse" || name == "rev") {
            return matrix_reverse(m1,has_error,e);
        } else if (name == "eye" || name == "id") {
            return matrix_eye(m1,has_error,e);
        } else if (name == "%") {
            return matrix_percent(m1);
        } else if (name == "tr" || name == "trace") {
            return matrix_trace(m1,has_error,e);
        } else if (get_function_argument_count(name) >= 2) {
            token t2 = argv[1];
            matrix m2 = *(matrix*)(t2.get_content());
            if (name == "+") {
                return matrix_add(m1,m2,has_error,e);
            } else if (name == "-") {
                return matrix_subtract(m1,m2,has_error,e);
            } else if (name == "*") {
                return matrix_mult(m1,m2,has_error,e);
            } else if (name == "/") {
                return matrix_div(m1,m2,has_error,e);
            } else if (name == "^") {
                return matrix_func_error_numonly(m1,m2,&number_pow,has_error,e);
            } else if (name == "log") {
                return matrix_func_error_numonly(m1,m2,&number_log,has_error,e);
            } else if (name == "range" || name == "~") {
                //return matrix_range(m1,m2,has_error,e);
                return matrix_func_numonly(m1,m2,&matrix_range,has_error,e);
            } else if (name == "one") {
                return matrix_one(m1,m2,has_error,e);
            } else if (name == "zero") {
                return matrix_zero(m1,m2,has_error,e);
            } else if (name == "append") {
                return matrix_append(m1,m2,has_error,e);
            } else if (name == "union") {
                return matrix_union(m1,m2);
            } else if (name == "intersection") {
                return matrix_intersection(m1,m2);
            } else if (name == "sym_diff") {
                return matrix_sym_diff(m1,m2);
            } else if (name == "rm" || name == "remove") {
                return matrix_remove(m1,m2);
            } else if (name == "mat_pow") {
                return matrix_pow(m1,m2,has_error,e);
            } else if (get_function_argument_count(name) >= 3) {
                token t3 = argv[2];
                matrix m3 = *(matrix*)(t3.get_content());
                if (name == "get") {
                    return matrix_get(m1,m2,m3,has_error,e);
                } else if (name == "resize") {
                    return matrix_resize(m1,m2,m3,has_error,e);
                } else if (get_function_argument_count(name) >= 4) {
                    token t4 = argv[3];
                    matrix m4 = *(matrix*)(t4.get_content());
                    if (name == "replace") {
                        return matrix_rep(m1,m2,m3,m4,has_error,e);
                    }
                }
            }
        }
    }
    //still not returned
    matrix m(0,0);
    has_error = true;
    e = error(error::ERROR_UNKNOWN_FUNC,name);
    return m;
}

matrix evaluate(const expression& exp, bool& has_error, error& e) {
    debug_print(std::cout << "eval expression: " << exp.get_string_representation()<< std::endl;)
    std::vector<token> tokens = exp.get_tokens();
    std::stack<token> arg_stack;
    for (const token& tok: tokens) {
        switch (tok.get_type()) {
            case token::FUNCTION:
            case token::SYMBOL:
            {
                size_t arg_count = get_function_argument_count(tok.get_string_representation());
                std::vector<token> argv;
                for (size_t i=0; i<arg_count; i++) {
                    if (arg_stack.empty()) {
                        has_error = true;
                        e = error(error::ERROR_WRONG_ARG_COUNT,tok.get_string_representation() + ": Expected " + num2str(arg_count) + " argument" + (arg_count==1?"":"s") + " but " + num2str(i) + (i==1?" was":" were") + " provided");
                        matrix m(0,0);
                        return m;
                    } else {
                        argv.push_back(arg_stack.top());
                        arg_stack.pop();
                    }
                }
                std::reverse(argv.begin(),argv.end());
                matrix result = evaluate_function(tok.get_string_representation(),argv,has_error,e);
                if (has_error) {
                    return result;
                }
                token t(result);
                arg_stack.push(t);
                break;
            }
                
            case token::NUMBER:
                arg_stack.push(tok.as_matrix_token());
                break;
                
            case token::MATRIX:
            {
                matrix* m = (matrix*)(tok.get_content());
                for (size_t i=0; i<m->row_count(); i++) {
                    for (size_t j=0; j<m->column_count(); j++) {
                        entry* ent = m->get(i,j);
                        expression* exp;
                        if ((exp = dynamic_cast<expression*>(ent)) != nullptr) {
                            matrix result = evaluate(*exp,has_error,e);
                            if (has_error) {
                                return result;
                            }
                            if ((result.row_count() == 1)&&(result.column_count() == 1)) {
                                m->set(i,j,*dynamic_cast<number*>(result.get(0,0)));
                            } else {
                                m->set(i,j,result);
                            }
                        }
                    }
                }
                arg_stack.push(tok);
            }
            break;
        }
    }
    if (arg_stack.size() == 1) {
        token t = arg_stack.top();
        has_error = false;
        if (t.get_type() == token::NUMBER) {
            return *(matrix*)(t.as_matrix_token().get_content());
        } else {
            return *(matrix*)(t.get_content());
        }
    } else {
        has_error = true;
        e = error(error::ERROR_WRONG_ARG_COUNT,"");
        matrix m(0,0);
        return m;
    }
}

void get_answer(std::string input, matrix& result, bool& has_error, error& e) {
    if (input.empty()) {
        has_error = true;
    } else {
        /*input = preprocess(input,has_error,e);
        if (has_error) {
            return;
        }*/
        expression tokenized_exp = tokenize(input,has_error,e);
        if (has_error) {
            return;
        } else {
            debug_print(std::cout << "trying to merge matrix\n";)
            tokenized_exp = merge_matrix(tokenized_exp,has_error,e);
            if (has_error) {
                return;
            }
            tokenized_exp = shunting_yard(tokenized_exp, has_error,e);
            if (has_error) {
                return;
            } else {
                matrix m = evaluate(tokenized_exp,has_error,e);
                if (has_error) {
                    return;
                } else {
                    result = m;
                }
            }
        }
    }
}

std::string calculate(std::string input) {
    if (input.empty()) {
        return "N/A";
    }
    matrix m(0,0);
    bool has_error = false;
    error e;
    size_t error_line;
    process_line(input,m,has_error,e,error_line);
    if (has_error) {
        return e.get_message() + "\nAt line: " + num2str(static_cast<int>(error_line)) + ".";
    } else {
        return m.get_string_representation();
    }
}

void reduceMemoryUsageCpp() {
    factorial.clear();
}

