//
//  matrix.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <utility>
#include <sstream>
#include "matrix.hpp"
#include "number.hpp"

matrix::matrix(size_t r, size_t c): row(r), column(c) {
    data = new entry**[r];
    for (size_t i=0; i<r; i++) {
        data[i] = new entry*[c];
        for (size_t j=0; j<c; j++) {
            data[i][j] = nullptr;
        }
    }
}

matrix::matrix(const matrix& mat): matrix(mat.row, mat.column) {
    for (size_t i=0; i<row; i++) {
        for (size_t j=0; j<column; j++) {
            entry* e = mat.get(i,j);
            data[i][j] = e->clone();
        }
    }
}

matrix::matrix(matrix&& mat): row(mat.row), column(mat.column) {
    this->data = mat.data;
    mat.data = nullptr;
}

matrix& matrix::operator=(matrix m) {
    std::swap(this->row,m.row);
    std::swap(this->column,m.column);
    std::swap(this->data,m.data);
    return *this;
}

matrix::~matrix() {
    if (data != nullptr) {
        for (size_t i=0; i<row; i++) {
            for (size_t j=0; j<column; j++) {
                delete data[i][j];
            }
            delete[] data[i];
        }
        delete[] data;
    }
}

entry* matrix::get(size_t _row, size_t _column) const {
    return data[_row][_column];
}

void matrix::set(size_t _row, size_t _column, const entry& e) {
    data[_row][_column] = e.clone();
}

size_t matrix::row_count() const {
    return row;
}

size_t matrix::column_count() const {
    return column;
}

entry* matrix::clone() const {
    return new matrix(*this);
}

std::string matrix::get_string_representation() const {
    if ((row == 1) && (column == 1)) {
        return data[0][0]->get_string_representation();
    } else {
        std::ostringstream oss;
        oss << "[";
        for (size_t i=0; i<row; i++) {
            for (size_t j=0; j<column; j++) {
                oss << data[i][j]->get_string_representation();
                if (j != column - 1) {
                    oss << ", ";
                } else if (i != row-1) {
                    oss << "; ";
                }
            }
        }
        oss << "]";
        return oss.str();
    }
}
