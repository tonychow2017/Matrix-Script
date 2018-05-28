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
