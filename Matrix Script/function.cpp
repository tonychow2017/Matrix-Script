//
//  function.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 15/3/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include "function.hpp"
#include <memory>

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
            e = error(error::ERROR_INTERNAL,"");
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
            e = error(error::ERROR_INTERNAL,"");
            return nullptr;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_INTERNAL,"");
        return nullptr;
    }
    return std::unique_ptr<entry>(result);
}

matrix matrix_add(matrix m1, const matrix& m2, bool& has_error, error& e) {
    if (m1.row_count() == m2.row_count()) {
        if (m1.column_count() == m2.column_count()) {
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
        } else {
            has_error = true;
            e = error(error::ERROR_DIM_MISMATCH,"");
        }
    } else {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH,"");
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
                    m.set(i,j,new_entry);
                }
            }
        }
    }
    return m;
}

matrix matrix_subtract(matrix m1, const matrix& m2, bool& has_error, error& e) {
    if (m1.row_count() == m2.row_count()) {
        if (m1.column_count() == m2.column_count()) {
            for (size_t i=0; i<m1.row_count(); i++) {
                for (size_t j=0; j<m1.column_count(); j++) {
                    entry* ent1 = m1.get(i,j);
                    entry* ent2 = m2.get(i,j);
                    number* num1, *num2;
                    matrix* mat1, *mat2;
                    if ((num1 = dynamic_cast<number*>(ent1)) != nullptr) {
                        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
                            m1.set(i,j,subtract(*num1,*num2));
                        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
                            m1.set(i,j,subtract(*num1,*mat2));
                        } else {
                            has_error = true;
                            e = error(error::ERROR_INTERNAL,"");
                        }
                    } else if ((mat1 = dynamic_cast<matrix*>(ent1)) != nullptr) {
                        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
                            m1.set(i,j,subtract(*mat1,*num2));
                        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
                            m1.set(i,j,matrix_subtract(*mat1,*mat2,has_error,e));
                            if (has_error) {
                                return m1;
                            }
                        }
                    }
                }
            }
            has_error = false;
        } else {
            has_error = true;
            e = error(error::ERROR_DIM_MISMATCH,"");
        }
    } else {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH,"");
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
            e = error(error::ERROR_INTERNAL,"");
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
            e = error(error::ERROR_INTERNAL,"");
            return nullptr;
        }
    } else {
        has_error = true;
        e = error(error::ERROR_INTERNAL,"");
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
    } else {
        has_error = true;
        e = error(error::ERROR_DIM_MISMATCH,"");
        return matrix(0,0);
    }
}

number inv(const number& n, bool& has_error, error& e) {
    double d = n.get_value();
    if (d == 0) {
        has_error = true;
        e = error(error::ERROR_DIV_BY_ZERO,"");
        return number(0);
    } else {
        has_error = false;
        return number(1.0/d);
    }
}
#include <iostream>
//Do Gaussian Elimination
double gaussian_elimination(matrix& m) {
    //ends when m becomes REF
    //step: permute rows so that those starting with 0 are at the bottom
    //if all rows start with 0, return 0
    //else, divide so that the first row starts with -1
    //for each non-zero starting row (except the first row), add the first row so that each becomes 0
    //repeat the step until the matrix becomes REF
    double det = 1;
    //std::cout << dynamic_cast<matrix*>(m.get(0,0)) << "\n";
    for (size_t first_row=0; first_row<m.row_count(); first_row++) {
        //find one non-zero row, then move it to top
        if ((dynamic_cast<number*>(m.get(first_row,first_row)))->get_value() == 0) {
            size_t i;
            for (i=first_row+1; i<m.row_count(); i++) {
                if ((dynamic_cast<number*>(m.get(i,first_row)))->get_value() != 0) {
                    //swap
                    m.exchange_rows(first_row,i);
                    std::cout << m.get_string_representation() << "\n";
                    det *= -1;
                    break;
                }
            }
            if (i == m.row_count()) {
                return 0;
            }
        }
        //now the first row must start with non-zero entry
        std::cout << m.get_string_representation() << "\n";
        std::cout << m.get(first_row,first_row)->get_string_representation() << "\n";
        number top_left = *dynamic_cast<number*>(m.get(first_row,first_row));
        det /= top_left.get_value();
        std::cout << top_left.get_string_representation() << "\n";
        for (size_t i=first_row+1; i<m.column_count(); i++) {
            number* entry = dynamic_cast<number*>(m.get(first_row,i));
            bool has_error = false;
            error e;
            number div = mult(*entry,inv(top_left,has_error,e));
            if (has_error) {
                //internal error?
            }
            m.set(first_row,i,div);
            std::cout << m.get_string_representation() << "\n";
        }
        number one(1);
        m.set(first_row,first_row,one);
        std::cout << m.get_string_representation() << "\n";
        for (size_t i=first_row+1; i<m.row_count(); i++) {
            if ((dynamic_cast<number*>(m.get(i,first_row)))->get_value() != 0) {
                number* multiply = dynamic_cast<number*>(m.get(i,first_row));
                for (size_t j=first_row+1; j<m.column_count(); j++) {
                    m.set(i,j, subtract(*dynamic_cast<number*>(m.get(i,j)),mult(*multiply,*dynamic_cast<number*>(m.get(first_row,j)))));
                }
                m.set(i,first_row,number(0));
            }
        }
        std::cout << m.get_string_representation() << "\n";
        //first_row++;
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
            return det;
        }
        std::cout << det;
        //subtract: previous rows
        for (size_t k=0; k<i; k++) {
            for (size_t l=first_non_zero_col+1; l<m.column_count(); l++) {
                m.set(k,l, subtract(*dynamic_cast<number*>(m.get(k,l)), mult(*dynamic_cast<number*>(m.get(k,first_non_zero_col)), *dynamic_cast<number*>(m.get(i,l)))));
                
                std::cout << m.get_string_representation() << "\n";
                
            }
            m.set(k,first_non_zero_col,number(0));
            std::cout << m.get_string_representation() << "\n";
        }
    }
    return det;
}

matrix matrix_inv(const matrix& m, bool& has_error, error& e) {
    std::cout << "---inv---\n";
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
        std::cout << "appended matrix\n";
        std::cout << new_mat.get_string_representation() << "\n";
        double det = gaussian_elimination(new_mat);
        if (det == 0) {
            has_error = true;
            e = error(error::ERROR_SINGULAR_MATRIX,"");
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
        e = error(error::ERROR_NON_SQUARE_MATRIX,"");
        return m;
    }
}
