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

matrix matrix_add(matrix m1, matrix& m2, bool& has_error, error& e);
matrix matrix_subtract(matrix m1, matrix& m2, bool& has_error, error& e);
matrix matrix_mult(const matrix& m1, const matrix& m2, bool& has_error, error& e);
matrix matrix_inv(const matrix& m, bool& has_error, error& e);
number matrix_det(matrix m, bool& has_error, error& e);
matrix matrix_rref(matrix m, bool& has_error, error& e);
number number_div(const number& n1, const number& n2, bool& has_error, error& e);
matrix matrix_div(const matrix& m1, const matrix& m2, bool& has_error, error& e);
number number_abs(const number& n);
number number_sin(const number& n);
number number_cos(const number& n);
number number_tan(const number& n, bool& has_error, error& e);
number number_csc(const number& n, bool& has_error, error& e);
number number_sec(const number& n, bool& has_error, error& e);
number number_cot(const number& n, bool& has_error, error& e);
number number_asin(const number& n, bool& has_error, error& e);
number number_acos(const number& n, bool& has_error, error& e);
number number_atan(const number& n);
number number_acsc(const number& n, bool& has_error, error& e);
number number_asec(const number& n, bool& has_error, error& e);
number number_acot(const number& n);
number number_log(const number& n1, const number& n2, bool& has_error, error& e);
number number_ln(const number& n, bool& has_error, error& e);
number number_ceil(const number& n);
number number_floor(const number& n);
number number_round(const number& n);
number number_pow(const number& n1, const number& n2, bool& has_error, error& e);
number number_exp(const number& n, bool& has_error, error& e);
matrix matrix_func(matrix m, number (*ptr)(const number&));
matrix matrix_func_error(matrix m, number (*ptr)(const number&, bool&, error&), bool& has_error, error& e);
matrix matrix_func_error_numonly(const matrix& m1, const matrix& m2, number (*ptr)(const number&, const number&, bool&, error&), bool& has_error, error& e);
number pi();
number matrix_row(const matrix&);
number matrix_col(const matrix&);
matrix matrix_size(const matrix&);
matrix matrix_sum(const matrix&, bool&, error&);
matrix matrix_product(const matrix&, bool&, error&);
number number_factorial(const number&, bool&, error&);

#endif /* function_hpp */
