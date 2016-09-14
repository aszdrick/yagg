/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include <ostream>
#include <map>

struct Interval {
    short low;
    short center_low;
    short center_high;
    short high;

    short center_distance(const Interval&) const;
    ushort size() const;
};

template<typename T>
class IntervalMap {
 public:
    using iterator = typename std::map<Interval, T>::iterator;
    using const_iterator = typename std::map<Interval, T>::const_iterator;
    using assoc = std::pair<Interval, T>;

    bool resize(const iterator&, Interval&);
    void split(const iterator&, Interval&, T&, bool = true);
    assoc premerge(const iterator&, Interval&);

    bool empty() const;
    auto count(const Interval&) const;
    auto find(const Interval&);
    template<typename U>
    auto insert(const U&);
    template<typename U>
    auto erase(U&);
    auto& operator[](const Interval&);

    auto begin();
    auto end();
    auto begin() const;
    auto end() const;

 private:
    std::map<Interval, T> map;
};

#include "Interval.ipp"

#endif /* INTERVAL_MAP */