/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

inline Interval Interval::unitary(unsigned short value) {
    return {value, value, value, value};
}

inline short Interval::center_distance(const Interval& iv) const {
    if (center_low >= iv.center_high)
        return center_low - iv.center_high - 1;
    else
        return iv.center_low - center_high - 1;
}

inline ushort Interval::size() const {
    return high - low + 1;
}

inline bool operator<(const Interval& lhs, const Interval& rhs) {
    return lhs.center_high < std::min(rhs.low, rhs.high);
}

inline std::ostream& operator<<(std::ostream& out, const Interval& iv) {
    out << "[" << iv.low << "(" << iv.center_low << ",";
    out << iv.center_high << ")" << iv.high << "]";
    return out;
}

inline unsigned short Interval::extract(const go::Position& position) const {
    if (position.row == center_low || position.row == center_high) {
        return position.column;
    } else if (position.column == center_low || position.column == center_high) {
        return position.row;
    }
    return -1;
}
