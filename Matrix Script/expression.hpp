//
//  expression.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef expression_hpp
#define expression_hpp

#include <vector>
#include "entry.hpp"
#include "token.hpp"

class expression: public entry {
public:
    std::string get_string_representation() const override;
    entry* clone() const override;
    expression(std::vector<token>);
    ~expression();
    const std::vector<token>& get_tokens() const;
    //matrix evaluate();
private:
    std::vector<token> tokens;
};

#endif /* expression_hpp */
