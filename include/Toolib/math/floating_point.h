// Markus Borris, 2015-17
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef FLOATING_POINT_H_INCL_09458t87t5twd2874t57rbn6
#define FLOATING_POINT_H_INCL_09458t87t5twd2874t57rbn6

#include "Toolib/assert.h"
#include "Toolib/comp_bwds.h"
#include "Toolib/ignore_arg.h"
#include "Toolib/PPDEFS.h"
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <limits>
#include <sstream>
#include <type_traits>


// I don't f***ing care restoring original defs for these...
#undef min
#undef max


namespace too
{
namespace math
{

//! \Returns true, if \params x and y are almost equal. Expects \param ulp >= 1.
template <typename FloatType>
typename std::enable_if<std::is_floating_point<FloatType>::value, bool>::type almost_equal(
    FloatType x, FloatType y, int ulp = 1)
{
    TOO_EXPECT(ulp >= 1);
    return std::abs(x - y) < std::numeric_limits<FloatType>::epsilon() * std::abs(x + y) * ulp ||
        std::abs(x - y) < std::numeric_limits<FloatType>::min();
}

//! Same as almost_equal, but also falls back to '==' if T is an integer type.
/** Useful if T is already a more general template parameter in your context.*/
template <typename FloatType>
typename std::enable_if<std::is_floating_point<FloatType>::value, bool>::type almost_equal_alltypes(
    FloatType x, FloatType y, int ulp = 1)
{
    return almost_equal(x, y, ulp);
}

//! Cf. other declaration of almost_equal_alltypes.
template <typename FloatType>
typename std::enable_if<std::is_integral<FloatType>::value, bool>::type almost_equal_alltypes(
    FloatType x, FloatType y, int ulp = 1)
{
    too::ignore_arg(ulp);
    return x == y;
}

//! Rough version of almost_equal, where you can pass a user defined eps(ilon) within which \params x and y are
//! understood approx. equal.
/** HINT: a common mistake leading to compiler error is not providing \param eps with explicit type FloatType, as the
 * other params.*/
template <typename FloatType>
typename std::enable_if<std::is_floating_point<FloatType>::value, bool>::type approx_equal(
    FloatType x, FloatType y, FloatType eps)
{
    return std::abs(x - y) < eps;
}

//! Cf. to_string functions.
enum class FloatFormat
{
    default_,
    fixed,
    scientific,
};

namespace impl
{
template <typename FloatType, FloatFormat FF>
struct ToStringConverter;
}

//! \Returns a string of the floating point number \param x.
/** \param precision controls either
        a) the count of significant digits for \param FF `default_`, or
        b) the decimal places for \param FF `fixed` or `scientific`.
    FF scientific leads to exponential formatting.
    If don't want to pass precision and use a default one (e.g. 6) and use FF default_, just use std::to_string.*/
template <FloatFormat FF = FloatFormat::default_, typename FloatType = double>
//  FloatType expected as floating point
typename std::enable_if<std::is_floating_point<FloatType>::value, std::string>::type to_string(
    FloatType x, int precision)
{
    TOO_EXPECT(precision >= 0);
    return impl::ToStringConverter<FloatType, FF>::convert(x, precision);
}

//! \Returns a string of the floating point number \param x.
/** \param FF selects the formatting: `default_` being equivalent to a call of std::to_string,
    `fixed` meaning fixed count of decimal places and `scientific` an exponential formatting.*/
template <FloatFormat FF = FloatFormat::default_, typename FloatType = double>
//  FloatType expected as floating point
typename std::enable_if<std::is_floating_point<FloatType>::value, std::string>::type to_string(FloatType x)
{
    return impl::ToStringConverter<FloatType, FF>::convert(x);
}


namespace impl
{
template <typename FloatType, FloatFormat FF>
struct ToStringConverter
{
};
template <typename FloatType>
struct ToStringConverter<FloatType, FloatFormat::default_>
{
    static std::string convert(FloatType x) { return too::to_string(x); }
    static std::string convert(FloatType x, int precision)
    {
        TOO_EXPECT(precision >= 0);
        std::ostringstream ret;
        ret << std::setprecision(precision) << x;
        return ret.str();
    }
};
template <typename FloatType>
struct ToStringConverter<FloatType, FloatFormat::fixed>
{
    static std::string convert(FloatType x)
    {
        std::ostringstream ret;
        ret << std::fixed << x;
        return ret.str();
    }
    static std::string convert(FloatType x, int precision)
    {
        TOO_EXPECT(precision >= 0);
        std::ostringstream ret;
        ret << std::fixed << std::setprecision(precision) << x;
        return ret.str();
    }
};
template <typename FloatType>
struct ToStringConverter<FloatType, FloatFormat::scientific>
{
    static std::string convert(FloatType x)
    {
        std::ostringstream ret;
        ret << std::scientific << x;
        return ret.str();
    }
    static std::string convert(FloatType x, int precision)
    {
        TOO_EXPECT(precision >= 0);
        std::ostringstream ret;
        ret << std::scientific << std::setprecision(precision) << x;
        return ret.str();
    }
};
} // impl
} // math
} // too

#endif
