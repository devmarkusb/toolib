// Markus Borris, 2016
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef DATE_TIME_H_sdhnfxeuhgf783nx4f2z3t487f
#define DATE_TIME_H_sdhnfxeuhgf783nx4f2z3t487f

#include <string>
#include <utility>
#include <ostream>
#include <vector>
#include "ToolibDEF.h"
#include "Toolib/string/string_token.h"
#include "Toolib/assert.h"


namespace too
{
namespace date_time
{

//! Months type, represents absolute as well as difference values.
using Months = int;
//! Years type, represents absolute as well as difference values.
using Years  = int;

//! Ensures month to be 1..12 and month and year having the same sign.
//! Only exception: {0, 0} will be mapped to {0, 0}.
struct TOOLIBSHARED_EXPORT normalize { static void do_it(std::pair<Years, Months>& y_m); };
//! Doesn't \ref normalize.
struct TOOLIBSHARED_EXPORT dont_normalize { static void do_it(std::pair<Years, Months>&) {} };

//! Contract: class is always in that normalized state if \ref normalize policy is chosen.
template <class NormalizePolicy = normalize>
class TOOLIBSHARED_EXPORT MonthYear_base;

//! Works as an absolute type as well as a difference type.
using MonthYear = MonthYear_base<>;

struct TOOLIBSHARED_EXPORT MonthYear_decl
{
    static const Months twelve = 12;
    static const std::string string_delim;
};

//! Works as an absolute type as well as a difference type.
template <class NormalizePolicy>
class TOOLIBSHARED_EXPORT MonthYear_base : public MonthYear_decl
{
public:
    MonthYear_base() = default;
    //! \params can have at least full int range.
    MonthYear_base(Months m, Years y);
    //! \param fromString must have form "<months><string_delim><years>",
    //! e.g. "2/2016", or "-387/235", full int range allowed. Please use
    //! MonthYear_decl::string_delim as delimiter (in the above examples defined as "/").
    explicit MonthYear_base(const std::string& fromString);

    //! Cf. suiting constructor MonthYear_base().
    MonthYear_base<NormalizePolicy>& operator=(const std::string& rhs);

    void set(Months m, Years y);
    Months asMonths() const;

    template <class NPR>
    MonthYear_base<NormalizePolicy>& operator-=(const MonthYear_base<NPR>& rhs);
    template <class NPR>
    MonthYear_base<NormalizePolicy>& operator+=(const MonthYear_base<NPR>& rhs);

    template <class NP>
    friend inline std::ostream& operator<<(std::ostream& out, const MonthYear_base<NP>& my);

    friend TOOLIBSHARED_EXPORT bool operator==(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
    friend TOOLIBSHARED_EXPORT bool operator!=(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
    friend TOOLIBSHARED_EXPORT bool operator<(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
    friend TOOLIBSHARED_EXPORT bool operator>(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
    friend TOOLIBSHARED_EXPORT bool operator<=(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
    friend TOOLIBSHARED_EXPORT bool operator>=(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);

private:
    std::pair<Years, Months> y_m; // order Years, Months because of lexicographical comparison capability of std::pair
};

const MonthYear_base<dont_normalize> one_year(0, 1);
static_assert(MonthYear_decl::twelve % 2 == 0, "");
const MonthYear_base<dont_normalize> half_year(MonthYear_decl::twelve / 2, 0);
const MonthYear_base<dont_normalize> quarter_year(MonthYear_decl::twelve / 4, 0);
const MonthYear_base<dont_normalize> one_month(1, 0);

TOOLIBSHARED_EXPORT MonthYear_base<> operator-(MonthYear_base<> lhs, const MonthYear_base<>& rhs);
TOOLIBSHARED_EXPORT MonthYear_base<> operator+(MonthYear_base<> lhs, const MonthYear_base<>& rhs);

//TOOLIBSHARED_EXPORT std::ostream& operator<<(std::ostream& out, const MonthYear_base<>& my);

TOOLIBSHARED_EXPORT bool operator==(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
TOOLIBSHARED_EXPORT bool operator!=(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
TOOLIBSHARED_EXPORT bool operator<(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
TOOLIBSHARED_EXPORT bool operator>(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
TOOLIBSHARED_EXPORT bool operator<=(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);
TOOLIBSHARED_EXPORT bool operator>=(const MonthYear_base<>& lhs, const MonthYear_base<>& rhs);



//#############################################################################################################
// template implementation

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy>::MonthYear_base(Months m, Years y)
    : y_m{y, m}
{
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy>::MonthYear_base(const std::string& fromString)
{
    std::vector<std::string> parts;
    too::str::tokenizeString(fromString, string_delim, parts);

    // don't throw the user an invalid_arg or sth., since bug could also have happened within tokenizeString
    TOO_ASSERT_THROW(parts.size() == 2);

    this->y_m.second = std::stoi(parts[0]);
    this->y_m.first  = std::stoi(parts[1]);
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
    this->y_m.first  = y;
    this->y_m.second = m;
    NormalizePolicy::do_it(this->y_m);
}

template <class NormalizePolicy>
Months MonthYear_base<NormalizePolicy>::asMonths() const
{
    return this->y_m.first * twelve + this->y_m.second;
}

template <class NPL>
template <class NPR>
MonthYear_base<NPL>& MonthYear_base<NPL>::operator-=(const MonthYear_base<NPR>& rhs)
{
    this->y_m.first -= rhs.y_m.first;
    this->y_m.second -= rhs.y_m.second;
    NPL::do_it(this->y_m);
    return *this;
}

template <class NPL>
template <class NPR>
MonthYear_base<NPL>& MonthYear_base<NPL>::operator+=(const MonthYear_base<NPR>& rhs)
{
    this->y_m.first += rhs.y_m.first;
    this->y_m.second += rhs.y_m.second;
    NPL::do_it(this->y_m);
    return *this;
}

template <class NormalizePolicy>
inline std::ostream& operator<<(std::ostream& out, const MonthYear_base<NormalizePolicy>& my)
{
    out << my.y_m.second << MonthYear_decl::string_delim << my.y_m.first;
    return out;
}


}
}

#endif
