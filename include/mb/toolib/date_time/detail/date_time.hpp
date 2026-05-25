//! \file

#ifndef DATE_TIME_HPP_KJGFDHNXUI3GFY34Z8RY27
#define DATE_TIME_HPP_KJGFDHNXUI3GFY34Z8RY27

#include "../../config.hpp"
#include "mb/ul/buildenv/comp_bwds.hpp"
#include "mb/ul/string/string_token.hpp"
#include "mb/ul/ul.hpp"
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace mb::too::date_time {
//####################################################################################################################
// template implementation

template <class NormalizePolicy>
template <class NP>
MonthYearBase<NormalizePolicy>::MonthYearBase(const MonthYearBase<NP>& other) {
    this->y_m_ = other.y_m;
    NormalizePolicy::do_it(this->y_m_);
}

template <class NormalizePolicy>
template <class NP>
MonthYearBase<NormalizePolicy>& MonthYearBase<NormalizePolicy>::operator=(const MonthYearBase<NP>& other) {
    MonthYearBase<NormalizePolicy> tmp(other);
    *this = std::move(tmp);
    return *this;
}

template <class NormalizePolicy>
template <class NP>
MonthYearBase<NormalizePolicy>::MonthYearBase(MonthYearBase<NP>&& other) {
    this->y_m_ = std::move(other.y_m);
    NormalizePolicy::do_it(this->y_m_);
}

template <class NormalizePolicy>
template <class NP>
MonthYearBase<NormalizePolicy>& MonthYearBase<NormalizePolicy>::operator=(MonthYearBase<NP>&& other) {
    this->y_m_ = std::move(other.y_m);
    NormalizePolicy::do_it(this->y_m_);
    return *this;
}

template <class NormalizePolicy>
MonthYearBase<NormalizePolicy>::MonthYearBase(const std::string& from_string) {
    std::vector<std::string> parts;
    ul::str::tokenize_string(from_string, string_delim, parts);

    // don't throw the user an invalid_arg or sth., since bug could also have happened within tokenize_string
    UL_ASSERT_THROW(parts.size() == 2);

    this->y_m_.second = ul::stoi(parts[0]);
    this->y_m_.first = ul::stoi(parts[1]);
    NormalizePolicy::do_it(this->y_m_);
}

template <class NormalizePolicy>
MonthYearBase<NormalizePolicy>& MonthYearBase<NormalizePolicy>::operator=(const std::string& rhs) {
    MonthYearBase<NormalizePolicy> tmp(rhs);
    *this = std::move(tmp);
    return *this;
}

template <class NormalizePolicy>
void MonthYearBase<NormalizePolicy>::set(Months m, Years y) {
    this->y_m_.first = y;
    this->y_m_.second = m;
    NormalizePolicy::do_it(this->y_m_);
}

template <class NormalizePolicy>
void MonthYearBase<NormalizePolicy>::set_months(Months m) {
    this->y_m_.second = m;
    NormalizePolicy::do_it(this->y_m_);
}

template <class NormalizePolicy>
void MonthYearBase<NormalizePolicy>::set_years(Years y) {
    this->y_m_.first = y;
    NormalizePolicy::do_it(this->y_m_);
}

template <class NormalizePolicy>
void MonthYearBase<NormalizePolicy>::get(Months& m, Years& y) const {
    m = this->y_m_.second;
    y = this->y_m_.first;
}

template <class NormalizePolicy>
Months MonthYearBase<NormalizePolicy>::get_months() const {
    return this->y_m_.second;
}

template <class NormalizePolicy>
Years MonthYearBase<NormalizePolicy>::get_years() const {
    return this->y_m_.first;
}

template <class NormalizePolicy>
Months MonthYearBase<NormalizePolicy>::as_months() const {
    return this->y_m_.first * twelve + this->y_m_.second;
}

template <class NormalizePolicy>
MonthYearBase<NormalizePolicy>& MonthYearBase<NormalizePolicy>::operator-=(const MonthYearBase<NormalizePolicy>& rhs) {
    this->y_m_.first -= rhs.y_m_.first;
    this->y_m_.second -= rhs.y_m_.second;
    NormalizePolicy::do_it(this->y_m_);
    return *this;
}

template <class NormalizePolicy>
MonthYearBase<NormalizePolicy>& MonthYearBase<NormalizePolicy>::operator+=(const MonthYearBase<NormalizePolicy>& rhs) {
    this->y_m_.first += rhs.y_m_.first;
    this->y_m_.second += rhs.y_m_.second;
    NormalizePolicy::do_it(this->y_m_);
    return *this;
}

template <class NormalizePolicy>
std::ostream& operator<<(std::ostream& out, const MonthYearBase<NormalizePolicy>& my) {
    out << my.y_m_.second << MonthYearDecl::string_delim << my.y_m_.first;
    return out;
}

template <class NormalizePolicy>
MonthYearBase<NormalizePolicy> operator-(
    MonthYearBase<NormalizePolicy> lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    lhs -= rhs;
    return lhs;
}

template <class NormalizePolicy>
MonthYearBase<NormalizePolicy> operator+(
    MonthYearBase<NormalizePolicy> lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    lhs += rhs;
    return lhs;
}

template <class NormalizePolicy>
bool operator==(const MonthYearBase<NormalizePolicy>& lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    return lhs.as_months() == rhs.as_months();
}

template <class NormalizePolicy>
bool operator!=(const MonthYearBase<NormalizePolicy>& lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    return !operator==(lhs, rhs);
}

template <class NormalizePolicy>
bool operator<(const MonthYearBase<NormalizePolicy>& lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    return lhs.as_months() < rhs.as_months();
}

template <class NormalizePolicy>
bool operator>(const MonthYearBase<NormalizePolicy>& lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    return operator<(rhs, lhs);
}

template <class NormalizePolicy>
bool operator<=(const MonthYearBase<NormalizePolicy>& lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    return !operator>(lhs, rhs);
}

template <class NormalizePolicy>
bool operator>=(const MonthYearBase<NormalizePolicy>& lhs, const MonthYearBase<NormalizePolicy>& rhs) {
    return !operator<(lhs, rhs);
}
} // namespace mb::too::date_time


#endif
