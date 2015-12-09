// Markus Borris, 2015
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef SCALE_H_INCL_enrhfu87n83464346ng
#define SCALE_H_INCL_enrhfu87n83464346ng

#include <utility>
#include <assert.h>
#include <type_traits>
#include <cmath>
#include <boost/optional.hpp>
#include "floating_point.h"


namespace too
{
namespace math
{

template <typename FloatingPointType>
class Map_LinearScale_Interval_to_Interval
{
public:
    Map_LinearScale_Interval_to_Interval(const std::pair<FloatingPointType, FloatingPointType>& FromInterval,
                                         const std::pair<FloatingPointType, FloatingPointType>& ToInterval)
        : m_FromInterval(FromInterval)
        , m_ToInterval(ToInterval)
    {
        assert(FromInterval.first < FromInterval.second);
        assert(ToInterval.first < ToInterval.second);
    }

    double operator()(const double& x)
    {
        return (x - m_FromInterval.first) * (m_ToInterval.second - m_ToInterval.first) / (m_FromInterval.second - m_FromInterval.first);
    }

private:
    std::pair<FloatingPointType, FloatingPointType> m_FromInterval;
    std::pair<FloatingPointType, FloatingPointType> m_ToInterval;
};

//! Calculates a meaningful step width (tick) for a scale with at most \param MaxTickCount tick markers suitable for a
//! data value range comprising \RangeMinToMax.
template <typename T>
//  requires T > 0
inline boost::optional<double> calcNiceScaleTick(T RangeMinToMax, unsigned long MaxTickCount)
{
    if (!MaxTickCount)
        return boost::none;
    const double MaxTickCount_ = static_cast<double>(MaxTickCount);
    const double MinimalTick = static_cast<double>(RangeMinToMax) / MaxTickCount_;
    const double magnitude = std::pow(10.0, std::floor(std::log10(MinimalTick)));
    if (too::math::almost_equal<T>(magnitude, 0.0))
        return boost::none;
    const double residual = MinimalTick / magnitude;
    if (residual > 5.0)
        return 10.0 * magnitude;
    else if (residual > 2.0)
        return 5.0 * magnitude;
    else if (residual > 1.0)
        return 2.0 * magnitude;
    else
        return magnitude;
}

}
}

#endif
