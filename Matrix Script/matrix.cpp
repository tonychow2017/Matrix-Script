//
//  matrix.cpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#include <utility>
#include <sstream>
#include <algorithm>
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

matrix::matrix(): matrix(0,0) {
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

bool matrix::operator<(const matrix& rhs) const {
    if (this->row_count()*this->column_count() < rhs.row_count()*rhs.column_count()) {
        return true;
    } else if (this->row_count()*this->column_count() > rhs.row_count()*rhs.column_count()) {
        return false;
    } else {
        if (this->row_count() < rhs.row_count()) {
            return true;
        } else if (this->row_count() > rhs.row_count()) {
            return false;
        } else {
            for (size_t i=0; i<this->row_count(); i++) {
                for (size_t j=0; j<this->column_count(); j++) {
                    number* n1, *n2;
                    matrix* m1, *m2;
                    if ((n1 = dynamic_cast<number*>(this->get(i,j))) != nullptr) {
                        if ((n2 = dynamic_cast<number*>(rhs.get(i,j))) != nullptr) {
                            bool is_smaller = n1 < n2;
                            if (is_smaller) {
                                return true;
                            } else if (n1 > n2) {
                                return false;
                            }
                        } else {
                            return true;
                        }
                    } else if ((m1 = dynamic_cast<matrix*>(this->get(i,j))) != nullptr) {
                        if ((n2 = dynamic_cast<number*>(rhs.get(i,j))) != nullptr) {
                            return false;
                        } else if ((m2 = dynamic_cast<matrix*>(rhs.get(i,j))) != nullptr) {
                            bool is_smaller = m1 < m2;
                            if (is_smaller) {
                                return true;
                            } else if (m1 > m2) {
                                return false;
                            }
                        } else {
                            return false;
                        }
                    } else {
                        return true;
                    }
                }
            }
            return false;
        }
    }
}

bool matrix::operator>(const matrix& rhs) const {
    return rhs < *this;
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

void matrix::exchange_rows(size_t row1, size_t row2, size_t starting_col) {
    if (row1 != row2) {
        entry** buffer = new entry*[column-starting_col];
        for (size_t i=starting_col; i<column; i++) {
            buffer[i-starting_col] = this->get(row1,i);
        }
        for (size_t i=starting_col; i<column; i++) {
            this->set(row1,i,*(this->get(row2,i)));
        }
        for (size_t i=starting_col; i<column; i++) {
            this->set(row2,i,*(buffer[i-starting_col]));
        }
        delete[] buffer;
    }
}

bool matrix::is_row_zero(size_t r) const {
    for (size_t i=0; i<this->column_count(); i++) {
        if (dynamic_cast<number*>(this->get(r,i))->get_value() != 0) {
            return false;
        }
    }
    return true;
}

bool matrix::is_singleton() const {
    return (row == 1) && (column == 1);
}

number* matrix::is_number_singleton() const {
    if (!this->is_singleton()) {
        return nullptr;
    } else {
        return dynamic_cast<number*>(this->get(0,0));
    }
}

bool matrix::is_empty() const {
    return (row == 0) || (column == 0);
}

std::string matrix::get_size_string() const {
    std::ostringstream oss;
    oss << "(" << row << ", " << column << ")";
    return oss.str();
}

void matrix::transpose() {
    entry*** new_data = new entry**[column];
    for (size_t i=0; i<column; i++) {
        new_data[i] = new entry*[row];
        for (size_t j=0; j<row; j++) {
            new_data[i][j] = data[j][i];
            data[j][i] = nullptr;
        }
    }
    entry*** old_data = data;
    data = new_data;
    if (old_data != nullptr) {
        for (size_t i=0; i<row; i++) {
            for (size_t j=0; j<column; j++) {
                delete old_data[i][j];
            }
            delete[] old_data[i];
        }
        delete[] old_data;
    }
    std::swap(row,column);
}

