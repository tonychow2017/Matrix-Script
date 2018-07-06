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
#include <set>
#include <cmath>
#include <utility>
#include <algorithm>
#include <cerrno>
#include <sstream>
#include <iostream>

std::string num2str(int i) {
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

std::string num2str(double i) {
    std::ostringstream oss;
    oss << i;
    return oss.str();
}

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
        e = error(error::ERROR_DIM_MISMATCH,std::string("Addition: ") + m1.get_size_string() + " and " + m2.get_size_string());
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
        e = error(error::ERROR_DIM_MISMATCH,std::string("Subtraction: ") + m1.get_size_string() + " and " + m2.get_size_string());
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
        e = error(error::ERROR_DIM_MISMATCH,std::string("Multiplication: ") + m1.get_size_string() + " and " + m2.get_size_string());
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
        e = error(error::ERROR_NON_SQUARE_MATRIX,std::string("Dimension: ") + m.get_size_string());
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
        e = error(error::ERROR_TRIGO,std::string("tan function evaluates to infinity"));
        return n;
    } else {
        return number(std::tan(n.get_value()));
    }
}

number number_csc(const number& n, bool& has_error, error& e) {
    double s = std::sin(n.get_value());
    if (s == 0) {
        has_error = true;
        e = error(error::ERROR_TRIGO,std::string("csc function evaluates to infinity"));
        return n;
    } else {
        return number(1.0/s);
    }
}

number number_sec(const number& n, bool& has_error, error& e) {
    double c = std::cos(n.get_value());
    if (c == 0) {
        has_error = true;
        e = error(error::ERROR_TRIGO,std::string("sec function evaluates to infinity"));
        return n;
    } else {
        return number(1.0/c);
    }
}

number number_cot(const number& n, bool& has_error, error& e) {
    double s = std::sin(n.get_value());
    if (s == 0) {
        has_error = true;
        e = error(error::ERROR_TRIGO,std::string("cot function evaluates to infinity"));
        return n;
    } else {
        return number(std::cos(n.get_value())/s);
    }
}

number number_asin(const number& n, bool& has_error, error& e) {
    double v = n.get_value();
    if ((v > 1)||(v < -1)) {
        has_error = true;
        e = error(error::ERROR_TRIGO,std::string("asin expects argument in [-1,1] but received ") + num2str(v));
        return n;
    } else {
        return number(std::asin(v));
    }
}

number number_acos(const number& n, bool& has_error, error& e) {
    double v = n.get_value();
    if ((v > 1)||(v < -1)) {
        has_error = true;
        e = error(error::ERROR_TRIGO,std::string("acos expects argument in [-1,1] but received ") + num2str(v));
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
    if (n1.get_value() <= 1) {
        has_error = true;
        e = error(error::ERROR_LOGARITHM,std::string("log expects base argument greater than 1 but received ") + num2str(n1.get_value()));
        return n1;
    } else if (n2.get_value() <= 0) {
        has_error = true;
        e = error(error::ERROR_LOGARITHM,std::string("log expects a positive argument but received ") + num2str(n2.get_value()));
        return n1;
    } else {
        return number(std::log(n2.get_value())/std::log(n1.get_value()));
    }
}

number number_ln(const number& n, bool& has_error, error& e) {
    if (n.get_value() <= 0) {
        has_error = true;
        e = error(error::ERROR_LOGARITHM,"ln expects a positive argument but received " + num2str(n.get_value()));
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

number number_sqrt(const number& n, bool& has_error, error& e) {
    double value = n.get_value();
    if (value < 0) {
        has_error = true;
        e = error(error::ERROR_POWER);
        return n;
    } else {
        return number(std::sqrt(value));
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

matrix matrix_func_numonly(const matrix& m1, const matrix& m2, matrix (*ptr)(const number&, const number&), bool& has_error, error& e) {
    if ((!check_if_all_number(m1)) || (!check_if_all_number(m2))) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
        return m1;
    } else if (m1.is_singleton() && m2.is_singleton()) {
        return ptr(*dynamic_cast<number*>(m1.get(0,0)), *dynamic_cast<number*>(m2.get(0,0)));
        //return result.as_matrix();
    } else if (m1.is_singleton()) {
        //m2 is not singleton
        matrix result(m2.row_count(),m2.column_count());
        number n1 = *dynamic_cast<number*>(m1.get(0,0));
        for (size_t i=0; i<m2.row_count(); i++) {
            for (size_t j=0; j<m2.column_count(); j++) {
                result.set(i,j,ptr(n1, *dynamic_cast<number*>(m2.get(i,j))));
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
                result.set(i,j,ptr(*dynamic_cast<number*>(m1.get(i,j)), n2));
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

number matrix_row(const matrix& m) {
    return number(m.row_count());
}

number matrix_col(const matrix& m) {
    return number(m.column_count());
}

matrix matrix_size(const matrix& m) {
    matrix result(1,2);
    result.set(0,0,number(m.row_count()));
    result.set(0,1,number(m.column_count()));
    return result;
}

matrix matrix_sum(const matrix& m, bool& has_error, error& e) {
    matrix* id = new matrix(1,1);
    id->set(0,0,number(0.0));
    std::unique_ptr<entry> ent(id);
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            ent = generic_add(ent.get(),m.get(i,j),has_error,e);
            if (has_error) {
                return *id;
            }
        }
    }
    if (dynamic_cast<number*>(ent.get()) != nullptr) {
        return dynamic_cast<number*>(ent.get())->as_matrix();
    } else {
        return *dynamic_cast<matrix*>(ent.get());
    }
}

matrix matrix_product(const matrix& m, bool& has_error, error& e) {
    matrix* id = new matrix(1,1);
    id->set(0,0,number(1.0));
    std::unique_ptr<entry> ent(id);
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            ent = generic_mult(ent.get(),m.get(i,j),has_error,e);
            if (has_error) {
                return *id;
            }
        }
    }
    if (dynamic_cast<number*>(ent.get()) != nullptr) {
        return dynamic_cast<number*>(ent.get())->as_matrix();
    } else {
        return *dynamic_cast<matrix*>(ent.get());
    }
}

number number_factorial(const number& n, bool& has_error, error& e) {
    if ((n.get_value() != std::round(n.get_value())) || (n.get_value() < 0)) {
        has_error = true;
        e = error(error::ERROR_NOT_NATURAL_NUMBER,n.get_string_representation());
        return n;
    } else {
        double prod = 1;
        for (size_t i=2; i<=std::round(n.get_value()); i++) {
            prod *= i;
        }
        return number(prod);
    }
}

matrix matrix_flatten(const matrix& m) {
    matrix flattened(1,m.row_count() * m.column_count());
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            flattened.set(0,i*m.row_count()+j,*(m.get(i,j)));
        }
    }
    return flattened;
}

matrix matrix_transpose(matrix m) {
    m.transpose();
    return m;
}

number matrix_min(const matrix& m, bool& has_error, error& e) {
    if (m.is_empty()) {
        has_error = true;
        e = error(error::ERROR_EMPTY_MATRIX);
        return number(0);
    } else if (!check_if_all_number(m)) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
        return number(0);
    }
    number* min = dynamic_cast<number*>(m.get(0,0));
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            number* current;
            if (*(current = dynamic_cast<number*>(m.get(i,j))) < *min) {
                min = current;
            }
        }
    }
    return *min;
}

number matrix_max(const matrix& m, bool& has_error, error& e) {
    if (m.is_empty()) {
        has_error = true;
        e = error(error::ERROR_EMPTY_MATRIX);
        return number(0);
    } else if (!check_if_all_number(m)) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
        return number(0);
    }
    number* max = dynamic_cast<number*>(m.get(0,0));
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            number* current;
            if (!(*(current = dynamic_cast<number*>(m.get(i,j))) < *max)) {
                max = current;
            }
        }
    }
    return *max;
}

matrix matrix_maxmin(const matrix& m, bool& has_error, error& e) {
    if (m.is_empty()) {
        has_error = true;
        e = error(error::ERROR_EMPTY_MATRIX);
        return matrix(0,0);
    } else if (!check_if_all_number(m)) {
        has_error = true;
        e = error(error::ERROR_ENTRY_NOT_ALL_NUMBER);
        return matrix(0,0);
    }
    number* max = dynamic_cast<number*>(m.get(0,0));
    number* min = max;
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            number* current;
            if (!(*(current = dynamic_cast<number*>(m.get(i,j))) < *max)) {
                max = current;
            } else if (*current < *min) {
                min = current;
            }
        }
    }
    matrix result(1,2);
    result.set(0,0,*max);
    result.set(0,1,*min);
    return result;
}

matrix matrix_range(const number& n1, const number& n2) {
    const number& smaller = std::min(n1,n2);
    const number& larger = std::max(n1,n2);
    double smaller_ceil = std::ceil(smaller.get_value());
    double larger_floor = std::floor(larger.get_value());
    matrix range(1,larger_floor-smaller_ceil+1);
    for (size_t i=0; i<(larger_floor-smaller_ceil+1); i++) {
        range.set(0,i,number(i+smaller_ceil));
    }
    return range;
}

bool is_smaller_entry(entry* ent1, entry* ent2) {
    matrix* m1, *m2;
    number* n1, *n2;
    if ((n1 = dynamic_cast<number*>(ent1)) != nullptr) {
        if ((n2 = dynamic_cast<number*>(ent2)) != nullptr) {
            return *n1 < *n2;
        } else {
            return true;
        }
    } else if ((m1 = dynamic_cast<matrix*>(ent1)) != nullptr) {
        if ((n2 = dynamic_cast<number*>(ent2)) != nullptr) {
            return false;
        } else if ((m2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
            return m1 < m2;
        } else {
            return true;
        }
    } else {
        return false;
    }
}

matrix matrix_sort(const matrix& m, bool& has_error, error& e) {
    if (m.row_count() != 1) {
        has_error = true;
        e = error(error::ERROR_NOT_ROW_VECTOR);
        return m;
    } else {
        if (m.column_count() <= 1) {
            return m;
        } else {
            entry** row = new entry*[m.column_count()];
            for (size_t i=0; i<m.column_count(); i++) {
                row[i] = m.get(0,i);
            }
            std::sort(row,row+m.column_count(),is_smaller_entry);
            matrix result(1,m.column_count());
            for (size_t i=0; i<m.column_count(); i++) {
                result.set(0,i,*(row[i]));
            }
            return result;
        }
    }
}

matrix matrix_eye_private(const number& n, bool& has_error, error& e) {
    double v = n.get_value();
    if (v != std::round(v) || v < 0) {
        has_error = true;
        e = error(error::ERROR_NOT_NATURAL_NUMBER);
        return matrix(0,0);
    } else if (v == 0) {
        return matrix(0,0);
    } else {
        size_t size = std::round(v);
        matrix result(size,size);
        for (size_t i=0; i<size; i++) {
            for (size_t j=0; j<size; j++) {
                result.set(i,j,number(i==j?1:0));
            }
        }
        return result;
    }
}

matrix matrix_eye(const matrix& m, bool& has_error, error& e) {
    number* num;
    if ((num = m.is_number_singleton()) == nullptr) {
        has_error = true;
        e = error(error::ERROR_NOT_NATURAL_NUMBER);
        return matrix(0,0);
    } else {
        return matrix_eye_private(*num, has_error, e);
    }
}

matrix matrix_fill(const number& n1, const number& n2, double value, bool& has_error, error& e) {
    double v1 = n1.get_value();
    double v2 = n2.get_value();
    if (v1 != std::round(v1) || v2 != std::round(v2) || v1 < 0 || v2 < 0) {
        has_error = true;
        e = error(error::ERROR_NOT_NATURAL_NUMBER);
        return matrix(0,0);
    } else if (v1 == 0 || v2 == 0) {
        return matrix(0,0);
    } else {
        size_t size1 = std::round(v1);
        size_t size2 = std::round(v2);
        matrix result(size1,size2);
        for (size_t i=0; i<size1; i++) {
            for (size_t j=0; j<size2; j++) {
                result.set(i,j,number(value));
            }
        }
        return result;
    }
}

matrix matrix_fill_value(const matrix& m1, const matrix& m2, double value, bool& has_error, error& e) {
    number* n1 = m1.is_number_singleton();
    number* n2 = m2.is_number_singleton();
    if (n1 == nullptr || n2 == nullptr) {
        has_error = true;
        e = error(error::ERROR_NOT_NUMBER);
        return matrix(0,0);
    } else {
        return matrix_fill(*n1,*n2,value,has_error,e);
    }
}

matrix matrix_zero(const matrix& m1, const matrix& m2, bool& has_error, error& e) {
    return matrix_fill_value(m1,m2,0,has_error,e);
}
        
matrix matrix_one(const matrix& m1, const matrix& m2, bool& has_error, error& e) {
    return matrix_fill_value(m1,m2,1,has_error,e);
}

matrix matrix_get(const matrix& m1, const matrix& m2, const matrix& m3, bool& has_error, error& e) {
    number* n2 = m2.is_number_singleton();
    number* n3 = m3.is_number_singleton();
    if (n2 == nullptr || n3 == nullptr) {
        has_error = true;
        e = error(error::ERROR_NOT_NUMBER);
        return matrix(0,0);
    } else {
        double v2 = n2->get_value();
        double v3 = n3->get_value();
        if (v2 < 0 || v3 < 0) {
            has_error = true;
            e = error(error::ERROR_NOT_NATURAL_NUMBER);
            return matrix(0,0);
        } else if (v2 != std::round(v2) || v3 != std::round(v3)) {
            has_error = true;
            e = error(error::ERROR_NOT_NATURAL_NUMBER);
            return matrix(0,0);
        } else if (v2 >= m1.row_count() || v3 >= m1.column_count()) {
            has_error = true;
            e = error(error::ERROR_OUT_OF_BOUNDARY,std::string("matrix has size ") + m1.get_size_string() + " but getting entry (" + num2str(std::round(v2)) + ", " + num2str(std::round(v3)) + ")");
            return matrix(0,0);
        } else {
            entry* ent = m1.get(std::round(v2),std::round(v3));
            matrix result(1,1);
            result.set(0,0,*ent);
            return result;
        }
    }
}

matrix matrix_rep(matrix m1, const matrix& m2, const matrix& m3, const matrix& m4, bool& has_error, error& e) {
    number* n2 = m2.is_number_singleton();
    number* n3 = m3.is_number_singleton();
    if (n2 == nullptr || n3 == nullptr) {
        has_error = true;
        e = error(error::ERROR_NOT_NUMBER);
        return matrix(0,0);
    } else {
        double v2 = n2->get_value();
        double v3 = n3->get_value();
        if (v2 < 0 || v3 < 0) {
            has_error = true;
            e = error(error::ERROR_NOT_NATURAL_NUMBER);
            return matrix(0,0);
        } else if (v2 != std::round(v2) || v3 != std::round(v3)) {
            has_error = true;
            e = error(error::ERROR_NOT_NATURAL_NUMBER);
            return matrix(0,0);
        } else if (v2 >= m1.row_count() || v3 >= m1.column_count()) {
            has_error = true;
            e = error(error::ERROR_OUT_OF_BOUNDARY,std::string("matrix has size ") + m1.get_size_string() + " but getting entry (" + num2str(std::round(v2)) + ", " + num2str(std::round(v3)) + ")");
            return matrix(0,0);
        } else {
            /*entry* ent = m1.get(std::round(v2),std::round(v3));
            matrix result(1,1);
            result.set(0,0,*ent);
            return result;*/
            m1.set(std::round(v2),std::round(v3),m4);
            return m1;
        }
    }
}

matrix matrix_append(const matrix& m1, const matrix& m2, bool& has_error, error& e) {
    if (m1.row_count() != m2.row_count()) {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH,std::string("Appending matrices of size ") + m1.get_size_string() + " and " + m2.get_size_string());
        return m1;
    } else {
        size_t col = m1.column_count() + m2.column_count();
        matrix result(m1.row_count(), col);
        for (size_t i=0; i<m1.row_count(); i++) {
            for (size_t j=0; j<m1.column_count(); j++) {
                result.set(i,j,*(m1.get(i,j)));
            }
        }
        for (size_t i=0; i<m2.row_count(); i++) {
            for (size_t j=0; j<m2.column_count(); j++) {
                result.set(i,j+m1.column_count(),*(m2.get(i,j)));
            }
        }
        return result;
    }
}

matrix force_as_matrix(entry* ent) {
    number* num;
    matrix* mat;
    if ((num = dynamic_cast<number*>(ent)) != nullptr) {
        return num->as_matrix();
    } else if ((mat = dynamic_cast<matrix*>(ent)) != nullptr) {
        return *mat;
    } else {
        return matrix(0,0);
    }
}

matrix matrix_unique(const matrix& m) {
    std::set<matrix> set;
    for (size_t i=0; i<m.row_count(); i++) {
        for (size_t j=0; j<m.column_count(); j++) {
            auto ele = (force_as_matrix(m.get(i,j)));
            set.emplace(ele);
        }
    }
    matrix result(1,set.size());
    size_t count = 0;
    for (const auto& ent: set) {
        result.set(0,count,ent);
        count++;
    }
    return result;
}

matrix matrix_union(const matrix& m1, const matrix& m2) {
    std::set<matrix> set;
    for (size_t i=0; i<m1.row_count(); i++) {
        for (size_t j=0; j<m1.column_count(); j++) {
            set.emplace(force_as_matrix(m1.get(i,j)));
        }
    }
    for (size_t i=0; i<m2.row_count(); i++) {
        for (size_t j=0; j<m2.column_count(); j++) {
            set.emplace(force_as_matrix(m2.get(i,j)));
        }
    }
    matrix result(1,set.size());
    size_t count = 0;
    for (const auto& ent: set) {
        result.set(0,count,ent);
        count++;
    }
    return result;
}

matrix matrix_intersection(const matrix& m1, const matrix& m2) {
    std::set<matrix> set1, set2, intersection;
    for (size_t i=0; i<m1.row_count(); i++) {
        for (size_t j=0; j<m1.column_count(); j++) {
            set1.emplace(force_as_matrix(m1.get(i,j)));
        }
    }
    for (size_t i=0; i<m2.row_count(); i++) {
        for (size_t j=0; j<m2.column_count(); j++) {
            set2.emplace(force_as_matrix(m2.get(i,j)));
        }
    }
    for (const auto& ent: set1) {
        if (set2.count(ent) == 1) {
            intersection.emplace(ent);
        }
    }
    matrix result(1,intersection.size());
    size_t count = 0;
    for (const auto& ent: intersection) {
        result.set(0,count,ent);
        count++;
    }
    return result;
}

matrix matrix_sym_diff(const matrix& m1, const matrix& m2) {
    std::set<matrix> set1, set2, symdiff;
    for (size_t i=0; i<m1.row_count(); i++) {
        for (size_t j=0; j<m1.column_count(); j++) {
            set1.emplace(force_as_matrix(m1.get(i,j)));
        }
    }
    for (size_t i=0; i<m2.row_count(); i++) {
        for (size_t j=0; j<m2.column_count(); j++) {
            set2.emplace(force_as_matrix(m2.get(i,j)));
        }
    }
    for (const auto& ent: set1) {
        if (set2.count(ent) == 0) {
            symdiff.emplace(ent);
        }
    }
    for (const auto& ent: set2) {
        if (set1.count(ent) == 0) {
            symdiff.emplace(ent);
        }
    }
    matrix result(1,symdiff.size());
    size_t count = 0;
    for (const auto& ent: symdiff) {
        result.set(0,count,ent);
        count++;
    }
    return result;

}

matrix matrix_resize_private(const matrix& m1, const number& n1, const number& n2, bool& has_error, error& e) {
    double v1 = n1.get_value();
    double v2 = n2.get_value();
    if (v1 <= 0 || v2 <= 0 || v1 != std::round(v1) || v2 != std::round(v2)) {
        has_error = true;
        e = error(error::ERROR_NOT_NATURAL_NUMBER);
        return m1;
    } else if (std::round(v1*v2) != m1.size()) {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH);
        return m1;
    } else {
        size_t row = std::round(v1);
        size_t col = std::round(v2);
        matrix result(row,col);
        for (size_t i=0; i<row; i++) {
            for (size_t j=0; j<col; j++) {
                size_t order = i*col + j;
                result.set(i,j,*(m1.get(order/m1.column_count(),order%m1.column_count())));
            }
        }
        return result;
    }
}

matrix matrix_resize(const matrix& m1, const matrix& m2, const matrix& m3, bool& has_error, error& e) {
    number* n1 = m2.is_number_singleton();
    number* n2 = m3.is_number_singleton();
    if (n1 == nullptr || n2 == nullptr) {
        has_error = true;
        e = error(error::ERROR_NOT_NUMBER);
        return m1;
    } else {
        return matrix_resize_private(m1, *n1, *n2, has_error, e);
    }
}


