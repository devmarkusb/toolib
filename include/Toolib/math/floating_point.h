// Markus Borris, 2015
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef FLOATING_POINT_H_INCL_09458t87t5twd2874t57rbn6
#define FLOATING_POINT_H_INCL_09458t87t5twd2874t57rbn6

#include <limits>
#include <cmath>
#include <type_traits>
#include <algorithm>
#include "Toolib/PPDEFS.h"

#if TOO_COMP_MS_VISUAL_STUDIO_CPP
#pragma push_macro("min")
#pragma push_macro("max")
#undef min
#undef max
#endif

namespace too
{
namespace math
{

template <typename T>
typename std::enable_if<!std::numeric_limits<T>::is_integer, bool>::type
    almost_equal(T x, T y, int ulp = 1)
{
    return std::abs(x-y) < std::numeric_limits<T>::epsilon() * std::abs(x+y) * ulp
           || std::abs(x-y) < std::numeric_limits<T>::min();
}

}
}

#if TOO_COMP_MS_VISUAL_STUDIO_CPP
#pragma pop_macro("max")
#pragma pop_macro("min")
#endif

#endif
