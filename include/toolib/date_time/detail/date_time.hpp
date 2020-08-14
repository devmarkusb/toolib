// Markus Borris, 2016
// This file is part of toolib library.

//!
/**
 */
//! \file

#ifndef DATE_TIME_HPP_kjgfdhnxui3gfy34z8ry27
#define DATE_TIME_HPP_kjgfdhnxui3gfy34z8ry27

#include "toolib/assert.h"
#include "toolib/comp_bwds.h"
#include "toolib/string/string_token.h"
#include <ostream>
#include <string>
#include <utility>
#include <vector>


namespace too
{
namespace date_time
{
//####################################################################################################################
// template implementation

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy>::MonthYear_base(Months m, Years y)
    : y_m{y, m}
{
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
template <class NP>
MonthYear_base<NormalizePolicy>::MonthYear_base(const MonthYear_base<NP>& other)
{
    this->y_m = other.y_m;
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
template <class NP>
MonthYear_base<NormalizePolicy>& MonthYear_base<NormalizePolicy>::operator=(const MonthYear_base<NP>& other)
{
    MonthYear_base<NormalizePolicy> tmp(other);
    *this = std::move(tmp);
    return *this;
}

template <class NormalizePolicy>
template <class NP>
MonthYear_base<NormalizePolicy>::MonthYear_base(MonthYear_base<NP>&& other)
{
    this->y_m = std::move(other.y_m);
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
template <class NP>
MonthYear_base<NormalizePolicy>& MonthYear_base<NormalizePolicy>::operator=(MonthYear_base<NP>&& other)
{
    this->y_m = std::move(other.y_m);
    NormalizePolicy::do_it(this->y_m);
    return *this;
}

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy>::MonthYear_base(const std::string& fromString)
{
    std::vector<std::string> parts;
    too::str::tokenizeString(fromString, string_delim, parts);

    // don't throw the user an invalid_arg or sth., since bug could also have happened within tokenizeString
    TOO_ASSERT_THROW(parts.size() == 2);

    this->y_m.second = too::stoi(parts[0]);
    this->y_m.first = too::stoi(parts[1]);
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy>& MonthYear_base<NormalizePolicy>::operator=(const std::string& rhs)
{
    MonthYear_base<NormalizePolicy> tmp(rhs);
    *this = std::move(tmp);
    return *this;
}

template <class NormalizePolicy>
void MonthYear_base<NormalizePolicy>::set(Months m, Years y)
{
    this->y_m.first = y;
    this->y_m.second = m;
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
void MonthYear_base<NormalizePolicy>::setMonths(Months m)
{
    this->y_m.second = m;
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
void MonthYear_base<NormalizePolicy>::setYears(Years y)
{
    this->y_m.first = y;
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
void MonthYear_base<NormalizePolicy>::get(Months& m, Years& y) const
{
    m = this->y_m.second;
    y = this->y_m.first;
}

template <class NormalizePolicy>
Months MonthYear_base<NormalizePolicy>::getMonths() const
{
    return this->y_m.second;
}

template <class NormalizePolicy>
Years MonthYear_base<NormalizePolicy>::getYears() const
{
    return this->y_m.first;
}

template <class NormalizePolicy>
Months MonthYear_base<NormalizePolicy>::asMonths() const
{
    return this->y_m.first * twelve + this->y_m.second;
}

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy>& MonthYear_base<NormalizePolicy>::operator-=(const MonthYear_base<NormalizePolicy>& rhs)
{
    this->y_m.first -= rhs.y_m.first;
    this->y_m.second -= rhs.y_m.second;
    NormalizePolicy::do_it(this->y_m);
    return *this;
}

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy>& MonthYear_base<NormalizePolicy>::operator+=(const MonthYear_base<NormalizePolicy>& rhs)
{
    this->y_m.first += rhs.y_m.first;
    this->y_m.second += rhs.y_m.second;
    NormalizePolicy::do_it(this->y_m);
    return *this;
}

template <class NormalizePolicy>
std::ostream& operator<<(std::ostream& out, const MonthYear_base<NormalizePolicy>& my)
{
    out << my.y_m.second << MonthYear_decl::string_delim << my.y_m.first;
    return out;
}

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy> operator-(
    MonthYear_base<NormalizePolicy> lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    lhs -= rhs;
    return lhs;
}

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy> operator+(
    MonthYear_base<NormalizePolicy> lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    lhs += rhs;
    return lhs;
}

template <class NormalizePolicy>
bool operator==(const MonthYear_base<NormalizePolicy>& lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    return lhs.asMonths() == rhs.asMonths();
}

template <class NormalizePolicy>
bool operator!=(const MonthYear_base<NormalizePolicy>& lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    return !operator==(lhs, rhs);
}

template <class NormalizePolicy>
bool operator<(const MonthYear_base<NormalizePolicy>& lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    return lhs.asMonths() < rhs.asMonths();
}

template <class NormalizePolicy>
bool operator>(const MonthYear_base<NormalizePolicy>& lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    return operator<(rhs, lhs);
}

template <class NormalizePolicy>
bool operator<=(const MonthYear_base<NormalizePolicy>& lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    return !operator>(lhs, rhs);
}

template <class NormalizePolicy>
bool operator>=(const MonthYear_base<NormalizePolicy>& lhs, const MonthYear_base<NormalizePolicy>& rhs)
{
    return !operator<(lhs, rhs);
}
} // namespace date_time
} // namespace too

#endif
