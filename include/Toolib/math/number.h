// Markus Borris, 2011
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef NUMBER_H_INCL_f29jh8hnf238hrxz23
#define NUMBER_H_INCL_f29jh8hnf238hrxz23

#include <string>
#include <iomanip>
#include <sstream>
#include <type_traits>
#include "Toolib/enum_cast.h"
#include "Toolib/assert.h"


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
    static_assert(std::is_integral<T>::value, "Only integral numbers are allowed as input");
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
}
}

#endif
