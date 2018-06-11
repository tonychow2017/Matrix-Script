//
//  function.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 15/3/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include "function.hpp"
#include <memory>
#include <vector>
#include <cmath>
#include <utility>
#include <algorithm>
#include <cerrno>
#include <iostream>

number add(const number& n1, const number& n2) {
    double v1 = n1.get_value();
    double v2 = n2.get_value();
    number result(v1+v2);
    return result;
}

matrix add(const number& n, matrix m) {
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            entry* ent = m.get(i,j);
            number* num;
            if ((num = dynamic_cast<number*>(ent)) != nullptr) {
                number new_entry = add(*num,n);
                m.set(i,j,new_entry);
            } else {
                matrix* mat;
                if ((mat = dynamic_cast<matrix*>(ent)) != nullptr) {
                    matrix new_entry = add(n,*mat);
                    m.set(i,j,new_entry);
                }
            }
        }
    }
    return m;
}

std::unique_ptr<entry> generic_add(entry* ent1, entry* ent2, bool& has_error, error& e) {
    number* num1, *num2;
    matrix* mat1, *mat2;
    entry* result;
    if ((num1 = dynamic_cast<number*>(ent1)) != nullptr) {
        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
            result = new number(add(*num1,*num2));
        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
            result = new matrix(add(*num1,*mat2));
        } else {
            has_error = true;
            e = error(error::ERROR_INTERNAL);
            return nullptr;
        }
    } else if ((mat1 = dynamic_cast<matrix*>(ent1)) != nullptr) {
        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
            result = new matrix(add(*num2,*mat1));
        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
            result = new matrix(matrix_add(*mat1,*mat2,has_error,e));
            if (has_error) {
                return nullptr;
            }
        } else {
            has_error = true;
            e = error(error::ERROR_INTERNAL);
            return nullptr;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_INTERNAL);
        return nullptr;
    }
    return std::unique_ptr<entry>(result);
}

matrix matrix_add(matrix m1, matrix& m2, bool& has_error, error& e) {
    if (m1.row_count() == m2.row_count() && m1.column_count() == m2.column_count()) {
        for (size_t i=0; i<m1.row_count(); i++) {
            for (size_t j=0; j<m1.column_count(); j++) {
                entry* ent1 = m1.get(i,j);
                entry* ent2 = m2.get(i,j);
                m1.set(i,j,*generic_add(ent1,ent2,has_error,e));
                if (has_error) {
                    return m1;
                }
            }
        }
        has_error = false;
    } else if (m1.is_singleton()) {
        number* n1 = dynamic_cast<number*>(m1.get(0,0));
        if (n1 != nullptr) {
            return add(*n1,m2);
        } else {
            has_error = true;
            return m1;
        }
    } else if (m2.is_singleton()) {
        number* n2 = dynamic_cast<number*>(m2.get(0,0));
        if (n2 != nullptr) {
            return add(*n2,m1);
        } else {
            has_error = true;
            return m1;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH);
    }
    return m1;
}

number subtract(const number& n1, const number& n2) {
    double v1 = n1.get_value();
    double v2 = n2.get_value();
    number result(v1-v2);
    return result;
}

matrix subtract(const number& n, matrix m) {
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            entry* ent = m.get(i,j);
            number* num;
            if ((num = dynamic_cast<number*>(ent)) != nullptr) {
                number new_entry = subtract(n,*num);
                m.set(i,j,new_entry);
            } else {
                matrix* mat;
                if ((mat = dynamic_cast<matrix*>(ent)) != nullptr) {
                    matrix new_entry = subtract(n,*mat);
                    m.set(i,j,new_entry);
                }
            }
        }
    }
    return m;
}

matrix subtract(matrix m, const number& n) {
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            entry* ent = m.get(i,j);
            number* num;
            if ((num = dynamic_cast<number*>(ent)) != nullptr) {
                number new_entry = subtract(*num,n);
                m.set(i,j,new_entry);
            } else {
                matrix* mat;
                if ((mat = dynamic_cast<matrix*>(ent)) != nullptr) {
                    matrix new_entry = subtract(*mat,n);
                }
            }
        }
    }
    return m;
}

std::unique_ptr<entry> generic_subtract(entry* ent1, entry* ent2, bool& has_error, error& e) {
    number* num1, *num2;
    matrix* mat1, *mat2;
    entry* result;
    if ((num1 = dynamic_cast<number*>(ent1)) != nullptr) {
        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
            result = new number(subtract(*num1,*num2));
        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
            result = new matrix(subtract(*num1,*mat2));
        } else {
            has_error = true;
            e = error(error::ERROR_INTERNAL);
            return nullptr;
        }
    } else if ((mat1 = dynamic_cast<matrix*>(ent1)) != nullptr) {
        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
            result = new matrix(subtract(*mat1,*num2));
        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
            result = new matrix(matrix_subtract(*mat1,*mat2,has_error,e));
            if (has_error) {
                return nullptr;
            }
        } else {
            has_error = true;
            e = error(error::ERROR_INTERNAL);
            return nullptr;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_INTERNAL);
        return nullptr;
    }
    return std::unique_ptr<entry>(result);
}

matrix matrix_subtract(matrix m1, matrix& m2, bool& has_error, error& e) {
    if (m1.row_count() == m2.row_count() && m1.column_count() == m2.column_count()) {
        for (size_t i=0; i<m1.row_count(); i++) {
            for (size_t j=0; j<m1.column_count(); j++) {
                entry* ent1 = m1.get(i,j);
                entry* ent2 = m2.get(i,j);
                m1.set(i,j,*generic_subtract(ent1,ent2,has_error,e));
                if (has_error) {
                    return m1;
                }
            }
        }
        has_error = false;
    } else if (m1.is_singleton()) {
        number* n1 = dynamic_cast<number*>(m1.get(0,0));
        if (n1 != nullptr) {
            return subtract(*n1,m2);
        } else {
            has_error = true;
            return m1;
        }
    } else if (m2.is_singleton()) {
        number* n2 = dynamic_cast<number*>(m2.get(0,0));
        if (n2 != nullptr) {
            return subtract(m1,*n2);
        } else {
            has_error = true;
            return m1;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH);
    }
    return m1;
}

number mult(const number& n1, const number& n2) {
    double v1 = n1.get_value();
    double v2 = n2.get_value();
    number result(v1*v2);
    return result;
}

matrix mult(const number& n, matrix m) {
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            entry* ent = m.get(i,j);
            number* num;
            if ((num = dynamic_cast<number*>(ent)) != nullptr) {
                number new_entry = mult(n,*num);
                m.set(i,j,new_entry);
            } else {
                matrix* mat;
                if ((mat = dynamic_cast<matrix*>(ent)) != nullptr) {
                    matrix new_entry = mult(n,*mat);
                    m.set(i,j,new_entry);
                }
            }
        }
    }
    return m;
}

std::unique_ptr<entry> generic_mult(entry* ent1, entry* ent2, bool& has_error, error& e) {
    number* num1, *num2;
    matrix* mat1, *mat2;
    entry* result;
    if ((num1 = dynamic_cast<number*>(ent1)) != nullptr) {
        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
            result = new number(mult(*num1,*num2));
        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
            result = new matrix(mult(*num1,*mat2));
        } else {
            has_error = true;
            e = error(error::ERROR_INTERNAL);
            return nullptr;
        }
    } else if ((mat1 = dynamic_cast<matrix*>(ent1)) != nullptr) {
        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
            result = new matrix(mult(*num2,*mat1));
        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
            result = new matrix(matrix_mult(*mat1,*mat2,has_error,e));
            if (has_error) {
                return nullptr;
            }
        } else {
            has_error = true;
            e = error(error::ERROR_INTERNAL);
            return nullptr;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_INTERNAL);
        return nullptr;
    }
    return std::unique_ptr<entry>(result);
}

matrix matrix_mult(const matrix& m1, const matrix& m2, bool& has_error, error& e) {
    if (m1.column_count() == m2.row_count()) {
        size_t sum_limit = m1.column_count();
        matrix resulting_mat(m1.row_count(),m2.column_count());
        for (size_t i=0; i<m1.row_count(); i++) {
            for (size_t j=0; j<m2.column_count(); j++) {
                //the (i,j)-th entry is defined to be
                //sum k from 0 to sum_limit
                //(i,k)*(k,j)
                std::unique_ptr<entry> sum(new number(0));
                for (size_t k=0; k<sum_limit; k++) {
                    std::unique_ptr<entry> product = generic_mult(m1.get(i,k),m2.get(k,j),has_error,e);
                    if (has_error) {
                        return resulting_mat;
                    } else {
                        sum = generic_add(product.get(),sum.get(),has_error,e);
                        if (has_error) {
                            return resulting_mat;
                        }
                    }
                }
                resulting_mat.set(i,j,*sum);
            }
        }
        has_error = false;
        return resulting_mat;
    } else if (m1.is_singleton()) {
        number* n1 = dynamic_cast<number*>(m1.get(0,0));
        if (n1 != nullptr) {
            return mult(*n1,m2);
        } else {
            has_error = true;
            return m1;
        }
    } else if (m2.is_singleton()) {
        number* n2 = dynamic_cast<number*>(m2.get(0,0));
        if (n2 != nullptr) {
            return mult(*n2,m1);
        } else {
            has_error = true;
            return m1;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH);
        return matrix(0,0);
    }
}

number inv(const number& n, bool& has_error, error& e) {
    double d = n.get_value();
    if (d == 0) {
        has_error = true;
        e = error(error::ERROR_DIV_BY_ZERO);
        return number(0);
    } else {
        has_error = false;
        return number(1.0/d);
    }
}

bool check_if_all_number(const matrix& m) {
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            entry* ent = m.get(i,j);
            if (dynamic_cast<number*>(ent) == nullptr) {
                return false;
            }
        }
    }
    return true;
}

double diagonal_product(const matrix& m) {
    double product = 1;
    for (size_t i=0; i<std::min(m.row_count(),m.column_count()); i++) {
        number* num = dynamic_cast<number*>(m.get(i,i));
        double value = num->get_value();
        if (value == 0) {
            return 0;
        } else {
            product *= value;
        }
    }
    return product;
}

//Do Gaussian Elimination
double gaussian_elimination(matrix& m, bool shortcut) {
    //ends when m becomes REF
    //step: permute rows so that those starting with 0 are at the bottom
    //if all rows start with 0, return 0
    //else, divide so that the first row starts with -1
    //for each non-zero starting row (except the first row), add the first row so that each becomes 0
    //repeat the step until the matrix becomes REF
    double det = 1;
    for (size_t first_row=0; first_row<m.row_count(); first_row++) {
        std::cout << "now on row " << first_row << "\n";
        //find one non-zero row, then move it to top
        //use partial pivoting
        std::vector<std::pair<size_t,size_t>> non_zero_row;
        size_t i;
        for (i=first_row; i<m.row_count(); i++) {
            if ((dynamic_cast<number*>(m.get(i,first_row)))->get_value() != 0) {
                non_zero_row.push_back(std::pair<size_t,size_t>(i,first_row));
            }
        }
        if (non_zero_row.empty()) {
            std::cout << "empty\n";
            continue;
        }
        //find largest abs value
        size_t largest_row = 0;
        double largest_value = 0;
        for (const auto& p: non_zero_row) {
            entry* ent = m.get(std::get<0>(p),std::get<1>(p));
            number* num = dynamic_cast<number*>(ent);
            double abs_value = std::abs(num->get_value());
            if (abs_value > largest_value) {
                largest_value = abs_value;
                largest_row = std::get<0>(p);
            }
        }
        m.exchange_rows(largest_row,first_row,first_row);
        std::cout << m.get_string_representation() << "\n";
        det = -det;
        //now the first row must start with non-zero entry
        number top_left = *dynamic_cast<number*>(m.get(first_row,first_row));
        det *= top_left.get_value();
        for (size_t i=first_row+1; i<m.column_count(); i++) {
            number* entry = dynamic_cast<number*>(m.get(first_row,i));
            bool has_error = false;
            error e;
            number div = mult(*entry,inv(top_left,has_error,e));
            if (has_error) {
                //internal error?
            }
            m.set(first_row,i,div);
        }
        number one(1);
        m.set(first_row,first_row,one);
        if (first_row != m.row_count()-1) {
            for (size_t i=first_row+1; i<m.row_count(); i++) {
                if ((dynamic_cast<number*>(m.get(i,first_row)))->get_value() != 0) {
                    number* multiply = dynamic_cast<number*>(m.get(i,first_row));
                    for (size_t j=first_row+1; j<m.column_count(); j++) {
                        m.set(i,j, subtract(*dynamic_cast<number*>(m.get(i,j)),mult(*multiply,*dynamic_cast<number*>(m.get(first_row,j)))));
                    }
                    m.set(i,first_row,number(0));
                    std::cout << m.get_string_representation() << "\n";
                }
            }
        }
    }
    std::cout << "ref done:\n   " << m.get_string_representation() << "\n";
    //note: REF is only partially done
    //cases like
    //[1 0 0 0;
    // 0 0 1 0;
    // 0 0 1 0;
    // 0 0 0 0]
    //are not simplified (and we don't need to do so)
    if (shortcut) {
        return det*diagonal_product(m);
    }
    //NOW make RREF
    //for each non-zero row, subtract above rows from that row
    //[1 2 3 4
    // 0 1 2 3
    // 0 0 1 2]
    for (size_t i=1; i<m.row_count(); i++) {
        size_t first_non_zero_col = 0;
        size_t j;
        for (j=1; j<m.column_count(); j++) {
            if ((dynamic_cast<number*>(m.get(i,j)))->get_value() != 0) {
                first_non_zero_col = j;
                break;
            }
        }
        if (j == m.column_count()) {
            //zero row
            return 0;
        }
        //subtract: previous rows
        for (size_t k=0; k<i; k++) {
            for (size_t l=first_non_zero_col+1; l<m.column_count(); l++) {
                m.set(k,l, subtract(*dynamic_cast<number*>(m.get(k,l)), mult(*dynamic_cast<number*>(m.get(k,first_non_zero_col)), *dynamic_cast<number*>(m.get(i,l)))));
            }
            m.set(k,first_non_zero_col,number(0));
            //at this stage, the row may become zero
            //if so, exchange with the last non-zero row
            if (m.is_row_zero(k)) {
                size_t q;
                for (q=m.column_count()-1; q>k; q--) {
                    if (!m.is_row_zero(q)) {
                        m.exchange_rows(q,k);
                        det = -det;
                        break;
                    }
                }
                if (q == k) {
                    return 0;
                } else {
                    i--;
                    break;
                }
            }
            std::cout << m.get_string_representation() << "\n";
        }
    }
    return det;
}

matrix matrix_inv(const matrix& m, bool& has_error, error& e) {
    if (!check_if_all_number(m)) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
        return m;
    }
    if (m.row_count() == m.column_count()) {
        //generate identity matrix; then append
        size_t size = m.row_count();
        matrix new_mat(size,size*2);
        for (size_t i=0; i<size; i++) {
            for (size_t j=0; j<size; j++) {
                if (i == j) {
                    new_mat.set(i,j+size,number(1));
                } else {
                    new_mat.set(i,j+size,number(0));
                }
                new_mat.set(i,j,*(m.get(i,j)));
            }
        }
        double det = gaussian_elimination(new_mat,false);
        if (det == 0) {
            has_error = true;
            e = error(error::ERROR_SINGULAR_MATRIX);
            return m;
        } else {
            matrix result(size,size);
            for (size_t i=0; i<size; i++) {
                for (size_t j=0; j<size; j++) {
                    result.set(i,j,*(new_mat.get(i,j+size)));
                }
            }
            return result;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_NON_SQUARE_MATRIX);
        return m;
    }
}

number matrix_det(matrix m, bool& has_error, error& e) {
    if (!check_if_all_number(m)) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
    }
    double det = gaussian_elimination(m,true);
    has_error = false;
    return number(det);
}

matrix matrix_rref(matrix m, bool& has_error, error& e) {
    if (!check_if_all_number(m)) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
    }
    gaussian_elimination(m,false);
    has_error = false;
    return m;
}

number number_abs(const number& n) {
    return number(std::abs(n.get_value()));
}

number number_sin(const number& n) {
    return number(std::sin(n.get_value()));
}

number number_cos(const number& n) {
    return number(std::cos(n.get_value()));
}

number number_tan(const number& n, bool& has_error, error& e) {
    if (number_cos(n).is_zero()) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(std::tan(n.get_value()));
    }
}

number number_csc(const number& n, bool& has_error, error& e) {
    double s = std::sin(n.get_value());
    if (s == 0) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(1.0/s);
    }
}

number number_sec(const number& n, bool& has_error, error& e) {
    double c = std::cos(n.get_value());
    if (c == 0) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(1.0/c);
    }
}

number number_cot(const number& n, bool& has_error, error& e) {
    double s = std::sin(n.get_value());
    if (s == 0) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(std::cos(n.get_value())/s);
    }
}

number number_asin(const number& n, bool& has_error, error& e) {
    double v = n.get_value();
    if ((v > 1)||(v < -1)) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(std::asin(v));
    }
}

number number_acos(const number& n, bool& has_error, error& e) {
    double v = n.get_value();
    if ((v > 1)||(v < -1)) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(std::acos(v));
    }
}

number number_atan(const number& n) {
    return std::atan(n.get_value());
}

number number_acsc(const number& n, bool& has_error, error& e) {
    double v = n.get_value();
    if ((v < 1)||(v > -1)) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(std::asin(1.0/v));
    }
}

number number_asec(const number& n, bool& has_error, error& e) {
    double v = n.get_value();
    if ((v < 1)||(v > -1)) {
        has_error = true;
        e = error(error::ERROR_TRIGO);
        return n;
    } else {
        return number(std::acos(1.0/v));
    }
}

number number_acot(const number& n) {
    return std::atan(1.0/n.get_value());
}

number number_log(const number& n1, const number& n2, bool& has_error, error& e) {
    if (n2.get_value() <= 1 || n2.get_value() <= 0) {
        has_error = true;
        e = error(error::ERROR_LOGARITHM);
        return n1;
    } else {
        return number(std::log(n2.get_value())/std::log(n1.get_value()));
    }
}

number number_ln(const number& n, bool& has_error, error& e) {
    if (n.get_value() <= 0) {
        has_error = true;
        e = error(error::ERROR_LOGARITHM);
        return n;
    } else {
        return number(std::log(n.get_value()));
    }
}

number number_ceil(const number& n) {
    return number(std::ceil(n.get_value()));
}

number number_floor(const number& n) {
    return number(std::floor(n.get_value()));
}

number number_round(const number& n) {
    return number(std::round(n.get_value()));
}

number number_pow(const number& n1, const number& n2, bool& has_error, error& e) {
    errno = 0;
    double result = std::pow(n1.get_value(),n2.get_value());
    if (errno != 0) {
        has_error = true;
        e = error(error::ERROR_POWER);
        return n1;
    } else {
        return number(result);
    }
}

number number_exp(const number& n, bool& has_error, error& e) {
    errno = 0;
    double v = std::exp(n.get_value());
    if (errno != 0) {
        has_error = true;
        e = error(error::ERROR_POWER);
        return n;
    } else {
        return number(v);
    }
}

number pi() {
    return number(std::atan(1)*4);
}

matrix matrix_func(matrix m, number (*ptr)(const number&)) {
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            m.set(i,j,ptr(*dynamic_cast<number*>(m.get(i,j))));
        }
    }
    return m;
}

matrix matrix_func_error(matrix m, number (*ptr)(const number&, bool&, error&), bool& has_error, error& e) {
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            m.set(i,j,ptr(*dynamic_cast<number*>(m.get(i,j)),has_error,e));
            if (has_error) {
                return m;
            }
        }
    }
    return m;
}

number number_div(const number& n1, const number& n2, bool& has_error, error& e) {
    if (n2.is_zero()) {
        has_error = true;
        e = error(error::ERROR_DIV_BY_ZERO);
        return n1;
    } else {
        return number(n1.get_value() / n2.get_value());
    }
}

matrix matrix_div(const matrix& m1, const matrix& m2, bool& has_error, error& e) {
    if (m1.is_singleton() && m2.is_singleton()) {
        if (check_if_all_number(m1) && check_if_all_number(m2)) {
            number result = number_div(*dynamic_cast<number*>(m1.get(0,0)),*dynamic_cast<number*>(m2.get(0,0)),has_error,e);
            return result.as_matrix();
        }
    }
    matrix inverted = matrix_inv(m2,has_error,e);
    if (has_error) {
        return m1;
    } else {
        matrix result = matrix_mult(m1,inverted,has_error,e);
        return result;
    }
}

matrix matrix_func_error_numonly(const matrix& m1, const matrix& m2, number (*ptr)(const number&, const number&, bool&, error&), bool& has_error, error& e) {
    if ((!check_if_all_number(m1)) || (!check_if_all_number(m2))) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
        return m1;
    } else if (m1.is_singleton() && m2.is_singleton()) {
        number result = ptr(*dynamic_cast<number*>(m1.get(0,0)), *dynamic_cast<number*>(m2.get(0,0)), has_error, e);
        return result.as_matrix();
    } else if (m1.is_singleton()) {
        //m2 is not singleton
        matrix result(m2.row_count(),m2.column_count());
        number n1 = *dynamic_cast<number*>(m1.get(0,0));
        for (size_t i=0; i<m2.row_count(); i++) {
            for (size_t j=0; j<m2.column_count(); j++) {
                result.set(i,j,ptr(n1, *dynamic_cast<number*>(m2.get(i,j)), has_error, e));
                if (has_error) {
                    return result;
                }
            }
        }
        return result;
    } else if (m2.is_singleton()) {
        //m1 is not singleton
        matrix result(m1.row_count(),m1.column_count());
        number n2 = *dynamic_cast<number*>(m2.get(0,0));
        for (size_t i=0; i<m1.row_count(); i++) {
            for (size_t j=0; j<m1.column_count(); j++) {
                result.set(i,j,ptr(*dynamic_cast<number*>(m1.get(i,j)), n2, has_error, e));
            }
        }
        return result;
    } else if (m1.is_empty() && m2.is_empty()) {
        return m1;
    } else {
        has_error = true;
        e = error(error::ERROR_NOT_NUMBER);
        return m1;
    }
}
