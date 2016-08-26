/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef BASE_MATRIX_HPP
#define BASE_MATRIX_HPP

#include <functional>
#include <vector>
#include <ostream>

using vector_iterator = typename std::vector<double>::iterator;
using const_vector_iterator = typename std::vector<double>::const_iterator;

class BaseMatrix {
 public:
    struct row {
        row(size_t, vector_iterator);
        row& operator++();
        row& operator--();
        row operator++(int);
        row operator--(int);
        row& operator=(double);
        row& operator=(std::initializer_list<double>);
        double& operator[](size_t);
        vector_iterator begin();
        vector_iterator end();
        bool operator==(const row&);
        bool operator!=(const row&);
     private:
        size_t size;
        vector_iterator iterator;
    };

    struct const_row {
        const_row(size_t, const_vector_iterator);
        const_row& operator++();
        const_row& operator--();
        const_row operator++(int);
        const_row operator--(int);
        double operator[](size_t) const;
        const_vector_iterator begin() const;
        const_vector_iterator end() const;
        bool operator==(const const_row&);
        bool operator!=(const const_row&);
     private:
        size_t size;
        const_vector_iterator iterator;
    };

    BaseMatrix();
    explicit BaseMatrix(size_t, size_t);
    BaseMatrix(std::initializer_list<std::initializer_list<double>>);

    row operator[](size_t);
    const_row operator[](size_t) const;

    size_t rowSize() const { return n_rows; }
    size_t columnSize() const { return n_columns; }

    row begin();
    row end();
    const_row begin() const;
    const_row end() const;

    BaseMatrix& operator+=(const BaseMatrix&);
    BaseMatrix& operator-=(const BaseMatrix&);
    BaseMatrix& operator*=(const BaseMatrix&);
    BaseMatrix operator+(const BaseMatrix&) const;
    BaseMatrix operator-(const BaseMatrix&) const;
    BaseMatrix operator*(const BaseMatrix&) const;

 private:
    size_t n_rows;
    size_t n_columns;
    std::vector<double> matrix;

    BaseMatrix& process(const BaseMatrix&,
                        const std::function<double(double,double)>&);

 public:
    template<unsigned N, unsigned M>
    static BaseMatrix make_matrix(const std::vector<double>& list) {
        BaseMatrix p;
        p.matrix = list;
        p.n_rows = N;
        p.n_columns = M;
        return p;
    }
    template<unsigned N>
    static BaseMatrix make_matrix(const std::vector<double>& list) {
        BaseMatrix p;
        p.matrix = list;
        p.n_rows = N;
        p.n_columns = list.size() / N;
        return p;
    }
};

std::ostream& operator<<(std::ostream&, const BaseMatrix&);

#endif
