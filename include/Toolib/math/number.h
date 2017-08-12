// Markus Borris, 2011-17
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef NUMBER_H_INCL_f29jh8hnf238hrxz23
#define NUMBER_H_INCL_f29jh8hnf238hrxz23

#include "Toolib/assert.h"
#include "Toolib/comp_bwds.h"
#include "Toolib/math/floating_point.h"
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

    /** Impl. notes:
            If you wonder, whether this could be implemented using std::modf instead of the rounding check,
            then no, that doesn't seem to be the right function here. It doesn't guarantee to round to the
            nearest integer. It only ensures that the sum of integral and fractional part gives the original
            value. Under mingw I got a test-case where 4.9999... did not yield 5 as integral part.
            Though one could improve the hard-coded 1e-12 (std::numeric_limits<long double>::min() is
            much too small).*/
    const long double exp         = std::log(x) / std::log(base);
    const long long intpart       = too::llround(exp);
    const long double intpart_dbl = too::narrow_cast<long double>(intpart);

    if (!too::math::approx_equal(intpart_dbl, exp, 1e-12L))
        return {};
    return too::narrow_cast<ArithType>(intpart);
}

} // math
} // too

#endif
