/* Copyright 2016 Ghabriel Nunes, Marleson Graf
   <ghabriel.nunes@gmail.com>, <aszdrick@gmail.com> */

#ifndef BASE_MATRIX_HPP
#define BASE_MATRIX_HPP

#include <functional>
#include <vector>
#include <ostream>


template<typename T>
class BaseMatrix {
    using vector_iterator = typename std::vector<T>::iterator;
    using const_vector_iterator = typename std::vector<T>::const_iterator;
    template<unsigned N, unsigned M, typename V>
    friend BaseMatrix<V> make_matrix(const std::vector<V>& list);
    template<unsigned N, typename V>
    friend BaseMatrix<V> make_matrix(const std::vector<V>& list);

 public:
    struct row {
        row(size_t, vector_iterator);
        row& operator++();
        row& operator--();
        row operator++(int);
        row operator--(int);
        row& operator=(T);
        row& operator=(std::initializer_list<T>);
        T& operator[](size_t);
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
        T operator[](size_t) const;
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
    BaseMatrix(std::initializer_list<std::initializer_list<T>>);

    row operator[](size_t);
    const_row operator[](size_t) const;

    size_t rowSize() const { return n_rows; }
    size_t columnSize() const { return n_columns; }

    vector_iterator begin();
    vector_iterator end();
    const_vector_iterator begin() const;
    const_vector_iterator end() const;

    BaseMatrix<T>& operator+=(const BaseMatrix<T>&);
    BaseMatrix<T>& operator-=(const BaseMatrix<T>&);
    BaseMatrix<T>& operator*=(const BaseMatrix<T>&);
    BaseMatrix<T> operator+(const BaseMatrix<T>&) const;
    BaseMatrix<T> operator-(const BaseMatrix<T>&) const;
    BaseMatrix<T> operator*(const BaseMatrix<T>&) const;

 private:
    size_t n_rows;
    size_t n_columns;
    std::vector<T> matrix;

    BaseMatrix<T>& process(const BaseMatrix<T>&,
                        const std::function<T(T,T)>&);
};

template<unsigned N, unsigned M, typename T = unsigned>
inline BaseMatrix<T> make_matrix(const std::vector<T>& list) {
    BaseMatrix<T> p;
    p.matrix = list;
    p.n_rows = N;
    p.n_columns = M;
    return p;
}

template<unsigned N, typename T = unsigned>
inline BaseMatrix<T> make_matrix(const std::vector<T>& list) {
    BaseMatrix<T> p;
    p.matrix = list;
    p.n_rows = N;
    p.n_columns = list.size() / N;
    return p;
}

template<typename T>
std::ostream& operator<<(std::ostream&, const BaseMatrix<T>&);

#include "BaseMatrix.ipp"

#endif
