/* Copyright 2016 Ghabriel Nunes <ghabriel.nunes@gmail.com>
                  Marleson Graf <aszdrick@gmail.com> */

template<typename T>
bool IntervalMap<T>::resize(const iterator& it, Interval& iv) {
    Interval piv = it->first;
    T& data = it->second;

    if (iv.center_low > piv.center_low && iv.center_high < piv.center_high) {
        return false;
    }

    if (piv.center_low > iv.center_high) {
        piv.low = iv.center_high + 1;
        iv.high = piv.center_low - 1;
    } else {
        piv.high = iv.center_low - 1;
        iv.low = piv.center_high + 1;
    }

    iterator hint = map.erase(it);
    map.insert(hint, {piv, data});
    return true;
}

template<typename T>
void IntervalMap<T>::split(const iterator& it, Interval& iv, T& value, bool lowSplit) {
    Interval upper = it->first;
    Interval lower = upper;
    T& data = it->second;

    lower.center_high = iv.low;
    lower.high = iv.center_low - 1;

    upper.center_low = iv.high;
    upper.low = iv.center_high + 1;

    iterator hint = map.erase(it);
    if (lowSplit) {
        map.insert(hint, {upper, data});
        map.insert({lower, value});
    } else {
        map.insert(hint, {upper, value});
        map.insert({lower, data});
    }
}

template<typename T>
typename IntervalMap<T>::assoc IntervalMap<T>::premerge(const iterator& it, Interval& iv) {
    Interval interval = it->first;
    T& data = it->second;

    // interval.low = std::min(interval.low, iv.low);
    // interval.high = std::max(interval.high, iv.high);

    // if (interval.center_low > iv.center_high) {
    //     interval.center_low = iv.center_low;
    // } else {
    //     interval.center_high = iv.center_high;
    // }

    map.erase(it);
    return {interval, data};
}

template<typename T>
auto IntervalMap<T>::count(const Interval& iv) const {
    return map.count(iv);
}

template<typename T>
auto& IntervalMap<T>::operator[](const Interval& iv) {
    return map[iv];
}

template<typename T>
auto IntervalMap<T>::insert(const auto& value) {
    return map.insert(value);
}

template<typename T>
auto IntervalMap<T>::find(const Interval& iv) {
    return map.find(iv);
}

template<typename T>
auto IntervalMap<T>::erase(auto& key) {
    return map.erase(key);
}

template<typename T>
auto IntervalMap<T>::begin() {
    return map.begin();
}

template<typename T>
auto IntervalMap<T>::end() {
    return map.end();
}

template<typename T>
auto IntervalMap<T>::begin() const {
    return map.cbegin();
}

template<typename T>
auto IntervalMap<T>::end() const {
    return map.cend();
}

inline short Interval::center_distance(const Interval& iv) const {
    if (center_low > iv.center_high)
        return center_low - iv.center_high - 1;
    else
        return iv.center_low - center_high - 1;
}

inline ushort Interval::size() const {
    return high - low;
}

inline bool operator<(const Interval& lhs, const Interval& rhs) {
    return lhs.center_high < std::min(rhs.low, rhs.high);
}

inline std::ostream& operator<<(std::ostream& out, const Interval& iv) {
    out << "[" << iv.low << "(" << iv.center_low << ",";
    out << iv.center_high << ")" << iv.high << "]";
    return out;
}
