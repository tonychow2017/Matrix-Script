//
//  matrix.hpp
//  Matrix Script
//
//  Created by Chun Wa Chow on 27/1/2018.
//  Copyright © 2018 Tony Chow. All rights reserved.
//

#ifndef matrix_hpp
#define matrix_hpp

#include <string>
#include "entry.hpp"
#include "number.hpp"

class number;

class matrix: public entry {
public:
    matrix();
    matrix(size_t row, size_t column);
    matrix(const matrix& mat);
    matrix(matrix&& mat);
    matrix& operator=(matrix m);
    bool operator<(const matrix&) const;
    bool operator>(const matrix&) const;
    ~matrix();
    entry* get(size_t row, size_t column) const;
    void set(size_t _row, size_t _column, const entry& data);
    size_t row_count() const;
    size_t column_count() const;
    std::string get_string_representation() const override;
    entry* clone() const override;
    void exchange_rows(size_t i, size_t j, size_t starting_col = 0);
    bool is_row_zero(size_t r) const;
    bool is_singleton() const;
    number* is_number_singleton() const;
    bool is_empty() const;
    std::string get_size_string() const;
    void transpose();
    
private:
    size_t row, column;
    entry*** data;
};

#endif /* matrix_hpp */
