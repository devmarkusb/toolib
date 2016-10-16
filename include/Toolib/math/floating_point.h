// Markus Borris, 2015
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef FLOATING_POINT_H_INCL_09458t87t5twd2874t57rbn6
#define FLOATING_POINT_H_INCL_09458t87t5twd2874t57rbn6

#include "Toolib/assert.h"
#include "Toolib\ignore_arg.h"
#include "Toolib/PPDEFS.h"
#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>


// I don't f***ing care restoring original defs for these...
#undef min
#undef max


namespace too
{
namespace math
{

//! \Returns true, if \params x and y are almost equal. Expects \param ulp >= 1.
template <typename T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal(T x, T y, int ulp = 1)
{
    TOO_EXPECT(ulp >= 1);
    return std::abs(x - y) < std::numeric_limits<T>::epsilon() * std::abs(x + y) * ulp ||
        std::abs(x - y) < std::numeric_limits<T>::min();
}

//! Same as almost_equal, but also falls back to '==' if T is an integer type.
/** Useful if T is already a more general template parameter in your context.*/
template <typename T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type almost_equal_alltypes(T x, T y, int ulp = 1)
{
    return almost_equal(x, y, ulp);
}

//! Cf. other declaration of almost_equal_alltypes.
template <typename T>
typename std::enable_if<std::numeric_limits<T>::is_integer, bool>::type almost_equal_alltypes(T x, T y, int ulp = 1)
{
    too::ignore_arg(ulp);
    return x == y;
}

//! Rough version of almost_equal, where you can pass a user defined eps(ilon) within which \params x and y are understood approx. equal.
template <typename T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type approx_equal(T x, T y, T eps)
{
    return std::abs(x - y) < eps;
}

}
}

#endif
