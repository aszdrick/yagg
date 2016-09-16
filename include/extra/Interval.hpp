/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

#ifndef INTERVAL_HPP
#define INTERVAL_HPP

#include <ostream>
#include <map>

struct Interval {
    unsigned short low;
    unsigned short center_low;
    unsigned short center_high;
    unsigned short high;

    static Interval unitary(unsigned short);
    short center_distance(const Interval&) const;
    ushort size() const;
};

#include "Interval.ipp"

#endif /* INTERVAL_MAP */
