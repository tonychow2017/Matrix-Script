//
//  main.cpp
//  Matrix Script CLI
//
//  Created by Chun Wa Chow on 1/2/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <iostream>
#include "main_calculator.hpp"

int main() {
    std::string input = "resize([2,3,4;1,2,3],6,1)";
    std::cout << calculate(input) << std::endl;
}

