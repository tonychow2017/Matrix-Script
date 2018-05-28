//
//  error.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 19/4/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include "error.hpp"

error::error(): type(error::NO_ERROR), message("") {
}

error::error(int t, std::string mess): type(t), message(mess) {
}

int error::get_type() {
    return type;
}

std::string error::get_message() {
    if (message != std::string("")) {
        return std::string("Error: ") + message;
    } else {
        return std::string("Error");
    }
}
