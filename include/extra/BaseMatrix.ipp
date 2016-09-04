/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#include <cassert>

//------------------------------- BaseMatrix --------------------------------//

template<typename T>
BaseMatrix<T>::BaseMatrix() : n_rows{3}, n_columns{3} {
    matrix.resize(9, T());
}

template<typename T>
BaseMatrix<T>::BaseMatrix(size_t n_rows, size_t n_columns)
: n_rows{n_rows}, n_columns{n_columns} {
    matrix.resize(n_rows * n_columns, T());
}

template<typename T>
BaseMatrix<T>::BaseMatrix(std::initializer_list<std::initializer_list<T>> l) {
    n_rows = l.size();
    n_columns = (*l.begin()).size();
    for (auto row : l) {
        for (auto value : row) {
            matrix.push_back(value);
        }
    }
}

template<typename T>
typename BaseMatrix<T>::row BaseMatrix<T>::operator[](size_t index) {
    return row{n_columns, matrix.begin() + (n_columns * index)};
}

template<typename T>
typename BaseMatrix<T>::const_row BaseMatrix<T>::operator[](size_t index) const {
    return const_row{n_columns, matrix.begin() + (n_columns * index)};
}

template<typename T>
typename BaseMatrix<T>::vector_iterator BaseMatrix<T>::begin() {
    // return row{n_columns, matrix.begin()}.iterator;
    return matrix.begin();
}

template<typename T>
typename BaseMatrix<T>::vector_iterator BaseMatrix<T>::end() {
    // return row{n_columns, matrix.end()}.iterator;
    return matrix.end();
}

template<typename T>
typename BaseMatrix<T>::const_vector_iterator BaseMatrix<T>::begin() const {
    // return const_row{n_columns, matrix.begin()}.iterator;
    return matrix.cbegin();
}

template<typename T>
typename BaseMatrix<T>::const_vector_iterator BaseMatrix<T>::end() const {
    // return const_row{n_columns, matrix.end()}.iterator;
    return matrix.cend();
}

template<typename T>
BaseMatrix<T>& BaseMatrix<T>::operator+=(const BaseMatrix<T>& rhs) {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    return process(rhs, std::plus<T>());
}

template<typename T>
BaseMatrix<T>& BaseMatrix<T>::operator-=(const BaseMatrix<T>& rhs) {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    return process(rhs, std::minus<T>());
}

template<typename T>
BaseMatrix<T>& BaseMatrix<T>::operator*=(const BaseMatrix<T>& rhs) {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    for (size_t i = 0; i < this->n_rows; i++) {
        for (size_t j = 0; j < rhs.n_columns; j++) {
            (*this)[i][j] = 0;
            for (size_t k = 0; k < this->n_columns; k++) {
                (*this)[i][j] += (*this)[i][k] * rhs[k][j];
            }
        }
    }
    return (*this);
}

template<typename T>
BaseMatrix<T> BaseMatrix<T>::operator+(const BaseMatrix<T>& rhs) const {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    BaseMatrix<T> temp = *this;
    return temp += rhs;
}

template<typename T>
BaseMatrix<T> BaseMatrix<T>::operator-(const BaseMatrix<T>& rhs) const {
    assert(this->n_rows == rhs.n_rows);
    assert(this->n_columns == rhs.n_columns);
    BaseMatrix<T> temp = *this;
    return temp -= rhs;
}

template<typename T>
BaseMatrix<T> BaseMatrix<T>::operator*(const BaseMatrix<T>& rhs) const {
    assert(this->n_columns == rhs.n_rows);
    BaseMatrix<T> temp(this->n_rows, rhs.n_columns);
    for (size_t i = 0; i < this->n_rows; i++) {
        for (size_t j = 0; j < rhs.n_columns; j++) {
            temp[i][j] = 0;
            for (size_t k = 0; k < this->n_columns; k++) {
                temp[i][j] += (*this)[i][k] * rhs[k][j];
            }
        }
    }
    return temp;
}

template<typename T>
BaseMatrix<T>& BaseMatrix<T>::process(const BaseMatrix<T>& m,
            const std::function<T(T,T)>& fn) {
    for (size_t i = 0; i < n_rows; i++) {
        for (size_t j = 0; j < n_columns; j++) {
            (*this)[i][j] = fn((*this)[i][j], m[i][j]);
        }
    }
    return *this;
}

template<typename T>
std::ostream& operator<<(std::ostream& out, const BaseMatrix<T>& matrix) {
    out << "[";
    for (size_t i = 0; i < matrix.rowSize(); i++) {
        if (i != 0) out << " ";
        for (size_t j = 0; j < matrix.columnSize(); j++) {
            if (j > 0) out << ", ";
            out << matrix[i][j];
        }
        if (i != matrix.rowSize() - 1) out << std::endl;
    }
    out << "]" << std::endl;
    return out;
}

//---------------------------------- row -----------------------------------//

template<typename T>
BaseMatrix<T>::row::row(size_t s, typename BaseMatrix<T>::vector_iterator v)
: size{s}, iterator{v} { }

template<typename T>
typename BaseMatrix<T>::row& BaseMatrix<T>::row::operator++() {
    iterator += size;
    return *this;
}

template<typename T>
typename BaseMatrix<T>::row& BaseMatrix<T>::row::operator--() {
    iterator -= size;
    return *this;
}

template<typename T>
typename BaseMatrix<T>::row BaseMatrix<T>::row::operator++(int) {
    typename BaseMatrix<T>::row old = *this;
    (*this)++;
    return old;
}

template<typename T>
typename BaseMatrix<T>::row BaseMatrix<T>::row::operator--(int) {
    typename BaseMatrix<T>::row old = *this;
    (*this)--;
    return old;
}

template<typename T>
typename BaseMatrix<T>::row& BaseMatrix<T>::row::operator=(T rhs) {
    *iterator = rhs;
    return *this;
}

template<typename T>
typename BaseMatrix<T>::row& BaseMatrix<T>::row::operator=(std::initializer_list<T> rhs) {
    assert(rhs.size() == size);
    size_t i = 0;
    for (auto value : rhs) {
        *(iterator + i) = value;
        i++;
    }
    return *this;
}

template<typename T>
T& BaseMatrix<T>::row::operator[](size_t index) {
    return *(iterator + index);
}

template<typename T>
typename BaseMatrix<T>::vector_iterator BaseMatrix<T>::row::begin() {
    return iterator;
}

template<typename T>
typename BaseMatrix<T>::vector_iterator BaseMatrix<T>::row::end() {
    return iterator + size;
}

template<typename T>
bool BaseMatrix<T>::row::operator==(const row& rhs) {
    return this->iterator == rhs.iterator;
}

template<typename T>
bool BaseMatrix<T>::row::operator!=(const row& rhs) {
    return !(*this == rhs);
}


//------------------------------- const_row --------------------------------//

template<typename T>
BaseMatrix<T>::const_row::const_row(size_t s, typename BaseMatrix<T>::const_vector_iterator v)
: size{s}, iterator{v} { }

template<typename T>
typename BaseMatrix<T>::const_row& BaseMatrix<T>::const_row::operator++() {
    iterator += size;
    return *this;
}

template<typename T>
typename BaseMatrix<T>::const_row& BaseMatrix<T>::const_row::operator--() {
    iterator -= size;
    return *this;
}

template<typename T>
typename BaseMatrix<T>::const_row BaseMatrix<T>::const_row::operator++(int) {
    typename BaseMatrix<T>::const_row old = *this;
    (*this)++;
    return old;
}

template<typename T>
typename BaseMatrix<T>::const_row BaseMatrix<T>::const_row::operator--(int) {
    typename BaseMatrix<T>::const_row old = *this;
    (*this)--;
    return old;
}

template<typename T>
T BaseMatrix<T>::const_row::operator[](size_t index) const {
    return *(iterator + index);
}

template<typename T>
typename BaseMatrix<T>::const_vector_iterator BaseMatrix<T>::const_row::begin() const {
    return iterator;
}

template<typename T>
typename BaseMatrix<T>::const_vector_iterator BaseMatrix<T>::const_row::end() const {
    return iterator + size;
}

template<typename T>
bool BaseMatrix<T>::const_row::operator==(const const_row& rhs) {
    return this->iterator == rhs.iterator;
}

template<typename T>
bool BaseMatrix<T>::const_row::operator!=(const const_row& rhs) {
    return !(*this == rhs);
}