// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef NUMBER_H_INCL_f29jh8hnf238hrxz23
#define NUMBER_H_INCL_f29jh8hnf238hrxz23

#include "Toolib/tooString.h"
#include "Toolib/enum_cast.h"

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
}
}

#endif
