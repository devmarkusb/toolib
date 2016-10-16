// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef RATIO_H_louiuzlik79hi965gi6
#define RATIO_H_louiuzlik79hi965gi6

#include "Toolib/assert.h"
#include <cstdint>
#include <initializer_list>
#include <string>
#include <type_traits>


namespace too
{
namespace math
{

//! Greatest common divisor. Expects at least on of \params a, b to be > 0.
template <typename T>
typename std::enable_if<std::is_integral<T>::value, T>::type gcd(T a, T b)
{
    TOO_EXPECT(a > T() || b > T());
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
typename std::enable_if<std::is_integral<T>::value, T>::type lcm(T a, T b)
{
    TOO_EXPECT(a > T() || b > T());
    return a * (b / gcd(a, b));
}


struct Rational;

//! Obviously ensures common denom of \params one, two after the call.
inline void make_common_denom(Rational& one, Rational& two);


//! Value type for rational numbers.
/** Since member denom is publicly accessible, the user is responsible
        a) to keep denom != 0 always, and
        b) to  represent negative numbers by setting num < 0 only,
            so denom is even expected to be > 0 always.
    Otherwise there is undefined behavior.*/
struct Rational
{
    using ValueType = std::intmax_t;

    //! Numerator.
    ValueType num{0};
    //! Denominator.
    ValueType denom{1}; // expected to be > 0 always

    //! Constructs a 0 (numerator 0, denominator 1).
    Rational() = default;

    //! Initialization expects either
    //!     a) exactly one numerator value (denominator being implicitly 1), or
    //!     b) two values, the first denoting the numerator and the second the denominator, or
    //!     c) an empty list, corresponding to numerator 0 and denominator 1.
    /** Otherwise behavior is undefined. As it is of course also, if denominator is passed as 0.
        Note the ugly implementaion detail: I need this init-list constructor to initialize conveniently
        because otherwise the default constructor seems to disturb.*/
    Rational(std::initializer_list<ValueType> init)
    {
        TOO_EXPECT(init.size() <= 2);
        auto it = init.begin();
        if (it == init.end())
        {
            num = {};
            return;
        }
        num = *it;
        ++it;
        if (it != init.end())
        {
            denom = *it;
            TOO_EXPECT(denom > 0);
        }
    }

    ~Rational()
    {
        // documenting the important invariant
        TOO_EXPECT(denom > 0);
    }

    //! Note that this ensures the invariant of having denom > 0 always
    void inverse()
    {
        std::swap(this->num, this->denom);
        if (this->denom < 0)
        {
            this->denom = -this->denom;
            this->num   = -this->num;
        }
        TOO_ENSURE(denom > 0);
    }

    template <typename T>
    typename std::enable_if<std::is_floating_point<T>::value, T>::type asFloatingPoint() const
    {
        return T(this->num) / T(this->denom);
    }

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
    one.num *= m / one.denom;
    one.denom = m;
    two.num *= m / two.denom;
    two.denom = m;

    TOO_ENSURE(one.denom == two.denom);
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
    if (lhs.denom == rhs.denom)
        return lhs.num == rhs.num;

    Rational l = lhs;
    Rational r = rhs;
    make_common_denom(l, r);
    return l.num == r.num;
}
inline bool operator!=(const Rational& lhs, const Rational& rhs) { return !operator==(lhs, rhs); }
inline bool operator<(const Rational& lhs, const Rational& rhs)
{
    if (lhs.denom == rhs.denom)
        return lhs.num < rhs.num;
    if (lhs.num == rhs.num)
        return lhs.denom > rhs.denom;

    Rational l = lhs;
    Rational r = rhs;
    make_common_denom(l, r);
    return l.num < r.num;
}
inline bool operator>(const Rational& lhs, const Rational& rhs) { return operator<(rhs, lhs); }
inline bool operator<=(const Rational& lhs, const Rational& rhs) { return !operator>(lhs, rhs); }
inline bool operator>=(const Rational& lhs, const Rational& rhs) { return !operator<(lhs, rhs); }


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
