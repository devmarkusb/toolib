// Markus Borris, 2015
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef SCALE_H_INCL_enrhfu87n83464346ng
#define SCALE_H_INCL_enrhfu87n83464346ng

#include <utility>
#include <type_traits>
#include <cmath>
#include "floating_point.h"
#include "Toolib/narrow.h"
#include "Toolib/assert.h"


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
        : m_FromInterval(FromInterval), m_ToInterval(ToInterval)
    {
        TOO_EXPECT(FromInterval.first < FromInterval.second);
        TOO_EXPECT(ToInterval.first < ToInterval.second);
    }

    double operator()(const double& x)
    {
        return (x - m_FromInterval.first) * (m_ToInterval.second - m_ToInterval.first) /
            (m_FromInterval.second - m_FromInterval.first);
    }

private:
    std::pair<FloatingPointType, FloatingPointType> m_FromInterval;
    std::pair<FloatingPointType, FloatingPointType> m_ToInterval;
};

//! Calculates a meaningful step width (tick) for a scale with at most \param MaxTickCount tick markers suitable for a
//! data value range comprising \RangeMinToMax.
template <typename T>
//  requires T > 0
inline double calcNiceScaleTick(T RangeMinToMax, unsigned long MaxTickCount)
{
    TOO_EXPECT(MaxTickCount);
    const double MaxTickCount_ = narrow<double>(MaxTickCount);
    const double MinimalTick   = narrow<double>(RangeMinToMax) / MaxTickCount_;
    const double magnitude = std::pow(10.0, std::floor(std::log10(MinimalTick)));
    assert(!too::math::almost_equal<T>(magnitude, 0.0));
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

//! Calculates a scale tick range, that contains \param minDataValue and \param maxDataValue at least, the individual
//! ticks being
//! \param scaleTick apart. \returns a pair of <min, max> tick values (min, max being a certain integer number of \param
//! scaleTick's apart).
template <typename T>
//  requires T number
inline std::pair<double, double> calcScaleTickFromTo(T minDataValue, T maxDataValue, double scaleTick)
{
    TOO_EXPECT(!almost_equal(scaleTick, 0.0));
    TOO_EXPECT(minDataValue <= maxDataValue);
    const double minIn  = narrow<double>(minDataValue);
    const double maxIn  = narrow<double>(maxDataValue);
    const double minOut = std::floor(minIn / scaleTick) * scaleTick;
    const double maxOut = std::ceil(maxIn / scaleTick) * scaleTick;
    return std::make_pair(minOut, maxOut);
}
}
}

#endif
