// Markus Borris, 2011-16
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef NUMBER_H_INCL_f29jh8hnf238hrxz23
#define NUMBER_H_INCL_f29jh8hnf238hrxz23

#include "Toolib/assert.h"
#include "Toolib/narrow.h"
#include "Toolib/enum_cast.h"
#include "Toolib/optional.h"
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <type_traits>


namespace too
{
namespace math
{
enum class ENumSys : unsigned char
{
    BIN = 2,
    OCT = 8,
    DEC = 10,
    HEX = 16,
};

template <typename T>
inline unsigned char getDigitCount(T Number, ENumSys Base = ENumSys::DEC)
{
    static_assert(std::is_integral<T>::value, "only integral numbers are allowed as input");
    unsigned char count = 0;
    do
    {
        ++count;
        Number /= as_number(Base);
    } while (Number != 0);
    return count;
}

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

template <typename T>
std::string toLeadingZeros(T x, int digits)
{
    static_assert(std::is_integral<T>::value, "");
    TOO_EXPECT_THROW(x >= T());
    TOO_EXPECT_THROW(digits >= 0);

    std::stringstream ss;
    ss << std::setw(digits) << std::setfill('0') << x;
    return ss.str();
}

//! \Returns the exponent with whom \param x is a power of \param base, if it is an integer one.
/** Expects parameters to be > 0 and base != 1.
    Note that the return value can be < 0, e.g. 0.5 is the -1 power of base 2.*/
template <typename ArithType>
//  ArithType expected to be is_arithmetic
too::opt<ArithType> is_power_of(ArithType x, ArithType base)
{
    static_assert(std::is_arithmetic<ArithType>::value, "only arithmetic numbers are allowed as input");
    TOO_EXPECT(x > ArithType{});
    TOO_EXPECT(base > ArithType{});
    TOO_EXPECT(base != static_cast<ArithType>(1));

    const long double exp = std::log(x) / std::log(base);
    long double intpart{};
    if (std::modf(exp, &intpart) != 0.0L)
        return {};
    return too::narrow_cast<ArithType>(intpart);
}

} // math
} // too

#endif
