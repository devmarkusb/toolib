// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef RATIO_H_louiuzlik79hi965gi6
#define RATIO_H_louiuzlik79hi965gi6

#include <cstdint>
#include <string>
#include <type_traits>
#include <initializer_list>
#include "Toolib/assert.h"


namespace too
{
namespace math
{

//! Greatest common divisor.
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
gcd(T a, T b)
{
    TOO_EXPECT_THROW(a != T() || b != T());
    T c = 0;
    while (a != 0)
    {
        c = a;
        a = b % a;
        b = c;
    }
    return b;
}

//! Lowest common multiple.
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type
lcm(T a, T b)
{
    return a * b / gcd(a, b);
}

struct Rational;
inline void make_common_denom(Rational& one, Rational& two);


//!
struct Rational
{
    using ValueType = std::intmax_t;

    ValueType num = 1;
    ValueType denom = 1;

    // ugly; I need this to initialize conveniently because otherwise the default constructor seems to disturb
    Rational(std::initializer_list<ValueType> init)
    {
        auto it = init.begin();
        num = *it;
        ++it;
        denom = *it;
    }

    void inverse() { std::swap(this->num, this->denom); }

    template <typename T = long double>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type
    asFloatingPoint() const { return T(this->num) / T(this->denom); }

    void reduce()
    {
        const auto d = gcd(this->num, this->denom);
        this->num /= d;
        this->denom /= d;
    }

    Rational& operator+=(const Rational& rhs)
    {
        Rational r = rhs;
        make_common_denom(*this, r);
        this->num += r.num;
        return *this;
    }

    Rational& operator-=(const Rational& rhs)
    {
        Rational r = rhs;
        make_common_denom(*this, r);
        this->num -= r.num;
        return *this;
    }

    Rational& operator*=(const Rational& rhs)
    {
        this->num *= rhs.num;
        this->denom *= rhs.denom;
        return *this;
    }

    Rational& operator/=(const Rational& rhs)
    {
        Rational r = rhs;
        r.inverse();
        *this *= r;
        return *this;
    }
};

inline void make_common_denom(Rational& one, Rational& two)
{
    const auto m = lcm(one.denom, two.denom);
    one.num*= one.denom / m;
    one.denom = m;
    two.num*= two.denom / m;
    two.denom = m;
}

inline Rational operator+(Rational lhs, const Rational& rhs)
{
    lhs += rhs;
    return lhs;
}
inline Rational operator-(Rational lhs, const Rational& rhs)
{
    lhs -= rhs;
    return lhs;
}
inline Rational operator*(Rational lhs, const Rational& rhs)
{
    lhs *= rhs;
    return lhs;
}
inline Rational operator/(Rational lhs, const Rational& rhs)
{
    lhs /= rhs;
    return lhs;
}

inline bool operator==(const Rational& lhs, const Rational& rhs)
{
    Rational l = lhs;
    Rational r = rhs;
    make_common_denom(l, r);
    return l.num == r.num && l.denom == r.denom;
}
inline bool operator!=(const Rational& lhs, const Rational& rhs)
{
    return !operator==(lhs, rhs);
}
inline bool operator<(const Rational& lhs, const Rational& rhs)
{
    Rational l = lhs;
    Rational r = rhs;
    make_common_denom(l, r);
    return l.num < r.num;
}
inline bool operator>(const Rational& lhs, const Rational& rhs)
{
    return operator<(rhs, lhs);
}
inline bool operator<=(const Rational& lhs, const Rational& rhs)
{
    return !operator>(lhs, rhs);
}
inline bool operator>=(const Rational& lhs, const Rational& rhs)
{
    return !operator<(lhs, rhs);
}


// clang-format off
const Rational atto  {1, 1000000000000000000};
const Rational femto {1, 1000000000000000};
const Rational pico  {1, 1000000000000};
const Rational nano  {1, 1000000000};
const Rational micro {1, 1000000};
const Rational milli {1, 1000};
const Rational centi {1, 100};
const Rational deci  {1, 10};
const Rational one   {1, 1};
const Rational deka  {10, 1};
const Rational hecto {100, 1};
const Rational kilo  {1000, 1};
const Rational mega  {1000000, 1};
const Rational giga  {1000000000, 1};
const Rational tera  {1000000000000, 1};
const Rational peta  {1000000000000000, 1};
const Rational exa   {1000000000000000000, 1};

const std::string  atto_symb = "a";
const std::string femto_symb = "f";
const std::string  pico_symb = "p";
const std::string  nano_symb = "n";
const std::string micro_symb = "\xC2\xB5";
const std::string milli_symb = "m";
const std::string centi_symb = "c";
const std::string  deci_symb = "d";
const std::string   one_symb = "";
const std::string  deka_symb = "da";
const std::string hecto_symb = "h";
const std::string  kilo_symb = "k";
const std::string  mega_symb = "M";
const std::string  giga_symb = "G";
const std::string  tera_symb = "T";
const std::string  peta_symb = "P";
const std::string   exa_symb = "E";

const Rational one_twelveth     {1, 12};
const Rational one_seventh      {1, 7};
const Rational one_twentyfourth {1, 24};
const Rational one_sixtyth      {1, 60};
const Rational sixtytimes       {60, 1};
const Rational twentyfourtimes  {24, 1};
const Rational seventimes       {7, 1};
const Rational twelvetimes      {12, 1};
// clang-format on

}
}

#endif
