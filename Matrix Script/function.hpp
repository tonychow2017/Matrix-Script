//
//  function.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 15/3/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef function_hpp
#define function_hpp

#include <cmath>
#include "matrix.hpp"
#include "number.hpp"
#include "token.hpp"
#include "error.hpp"

matrix matrix_add(matrix m1, const matrix& m2, bool& has_error, error& e);
matrix matrix_subtract(matrix m1, const matrix& m2, bool& has_error, error& e);
matrix matrix_mult(const matrix& m1, const matrix& m2, bool& has_error, error& e);
matrix matrix_inv(const matrix& m, bool& has_error, error& e);
number matrix_det(matrix m, bool& has_error, error& e);
matrix matrix_rref(matrix m, bool& has_error, error& e);

#endif /* function_hpp */
