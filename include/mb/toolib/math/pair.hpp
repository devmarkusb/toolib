//! \file

#ifndef PAIR_HPP_X2UR832RX823R8R
#define PAIR_HPP_X2UR832RX823R8R

#include "../config.hpp"
#include <utility>

namespace mb::too::math {
template <typename T1, typename T2>
struct Pair {
    Pair()
        : m_p_()
        , left(getleft())
        , right(getright()) {
    }

    Pair(const T1& l, const T2& r)
        : m_p_(l, r)
        , left(getleft())
        , right(getright()) {
    }

    template <typename OT1, typename OT2>
    explicit Pair(const Pair<OT1, OT2>& p)
        : m_p_(p)
        , left(getleft())
        , right(getright()) {
    }

    template <typename OT1, typename OT2>
    Pair<T1, T2>& operator=(const Pair<OT1, OT2>& p) {
        Pair<T1, T2> temp(p);
        temp.swap(*this);
        return *this;
    }

    template <typename OT1, typename OT2>
    Pair<T1, T2>& operator=(const std::pair<OT1, OT2>& p) {
        Pair<T1, T2> temp(p);
        temp.swap(*this);
        return *this;
    }

    template <typename OT1, typename OT2>
    explicit Pair(const std::pair<OT1, OT2>& p)
        : m_p_(p)
        , left(getleft())
        , right(getright()) {
    }

    template <typename OT1, typename OT2>
    explicit operator std::pair<OT1, OT2>() const {
        return m_p_;
    }

    template <typename OT1, typename OT2>
    Pair<T1, T2>& operator+=(const Pair<OT1, OT2>& p) {
        m_p_.first += p.left;
        m_p_.second += p.right;
        return *this;
    }

    template <typename OT1, typename OT2>
    Pair<T1, T2>& operator+=(const std::pair<OT1, OT2>& p) {
        m_p_.first += p.first;
        m_p_.second += p.second;
        return *this;
    }

    void swap(Pair<T1, T2>& p) {
        m_p_.swap(p);
    }

    Pair<T1, T2> flip() {
        return Pair<T1, T2>(m_p_.second, m_p_.first);
    }

    T1& left;
    T2& right;

private:
    std::pair<T1, T2> m_p_;

    T1& getleft() {
        return m_p_.first;
    }

    T2& getright() {
        return m_p_.second;
    }
};
} // namespace mb::too::math


#endif
