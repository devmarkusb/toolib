// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef ROUND_H_INCL_9mj238fz23nf8z
#define ROUND_H_INCL_9mj238fz23nf8z

#include <cmath>
#include <limits>
#include <type_traits>


namespace too
{
namespace math
{
inline double round(double r, unsigned short decimal_places)
{
    double factor = decimal_places ? pow(10.0, static_cast<double>(decimal_places)) : 1.0;
    return (r >= 0.0) ? floor(r * factor + 0.5) / factor : ceil(r * factor - 0.5) / factor;
}

template<typename T> inline T round_to(double r, unsigned short decimal_places = 0)
{
	const volatile bool is_T_integral = std::is_integral<T>::value;
	if (is_T_integral)
        decimal_places = 0; // for integral target values decimal_places make no sense
    using std::numeric_limits;
    const double d = round(r, decimal_places);
    if (d > numeric_limits<T>::max())
        return numeric_limits<T>::max();
    else if (d < numeric_limits<T>::min())
        return numeric_limits<T>::min();
    return static_cast<T>(d);
}
}
}

#endif
