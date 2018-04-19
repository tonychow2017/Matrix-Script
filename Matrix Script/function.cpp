//
//  function.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 15/3/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include "function.hpp"

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

matrix matrix_add(matrix m1, const matrix& m2, bool& has_error, error& e) {
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
                            m1.set(i,j,add(*num1,*num2));
                        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
                            m1.set(i,j,add(*num1,*mat2));
                        } else {
                            has_error = true;
                            e = error(error::ERROR_INTERNAL,"");
                        }
                    } else if ((mat1 = dynamic_cast<matrix*>(ent1)) != nullptr) {
                        if ((num2 = dynamic_cast<number*>(ent2)) != nullptr) {
                            m1.set(i,j,add(*num2,*mat1));
                        } else if ((mat2 = dynamic_cast<matrix*>(ent2)) != nullptr) {
                            m1.set(i,j,matrix_add(*mat1,*mat2,has_error,e));
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
