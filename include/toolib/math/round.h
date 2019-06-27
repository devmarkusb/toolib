// Markus Borris, 2011-16
// This file is part of toolib library.

//!
/**
*/
//! \file


#ifndef ROUND_H_9mj238fz23nf8z
#define ROUND_H_9mj238fz23nf8z

#include <cmath>
#include <limits>
#include <type_traits>

#include "toolib/PPDEFS/UNDEF_MIN_MAX"

namespace too
{
namespace math
{

template <typename T>
inline T round(T r, unsigned short decimal_places)
{
    static_assert(std::is_floating_point<T>::value, "");
    T factor = decimal_places ? pow(10.0, static_cast<T>(decimal_places)) : 1.0;
    return (r >= 0.0) ? floor(r * factor + 0.5) / factor : ceil(r * factor - 0.5) / factor;
}

template <typename TR, typename TP>
inline TR round_to(TP r, unsigned short decimal_places = 0)
{
    const volatile bool is_TR_integral = std::is_integral<TR>::value;
    if (is_TR_integral)
        decimal_places = 0; // for integral target values decimal_places make no sense
    using std::numeric_limits;
    const TP d = round(r, decimal_places);
    if (d > numeric_limits<TR>::max())
        return numeric_limits<TR>::max();
    else if (d < numeric_limits<TR>::min())
        return numeric_limits<TR>::min();
    return static_cast<TR>(d);
}
}
}

#endif
