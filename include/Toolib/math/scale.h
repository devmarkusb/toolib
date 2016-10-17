// Markus Borris, 2015-16
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef SCALE_H_INCL_enrhfu87n83464346ng
#define SCALE_H_INCL_enrhfu87n83464346ng

#include "floating_point.h"
#include "Toolib/assert.h"
#include "Toolib/narrow.h"
#include <cmath>
#include <type_traits>
#include <utility>


namespace too
{
namespace math
{

template <typename FloatingPointType>
class Map_LinearScale_Interval_to_Interval
{
    static_assert(std::is_floating_point<FloatingPointType>::value, "floating point type expected");

public:
    Map_LinearScale_Interval_to_Interval(const std::pair<FloatingPointType, FloatingPointType>& FromInterval,
        const std::pair<FloatingPointType, FloatingPointType>& ToInterval)
        : m_FromInterval(FromInterval), m_ToInterval(ToInterval)
    {
        TOO_EXPECT_THROW(FromInterval.first < FromInterval.second);
        TOO_EXPECT_THROW(ToInterval.first < ToInterval.second);
    }

    FloatingPointType operator()(const FloatingPointType& from) const
    {
        return (from - m_FromInterval.first) * (m_ToInterval.second - m_ToInterval.first) /
            (m_FromInterval.second - m_FromInterval.first);
    }

    FloatingPointType inverse(const FloatingPointType& to) const
    {
        return to * (m_FromInterval.second - m_FromInterval.first) / (m_ToInterval.second - m_ToInterval.first) +
            m_FromInterval.first;
    }

private:
    std::pair<FloatingPointType, FloatingPointType> m_FromInterval;
    std::pair<FloatingPointType, FloatingPointType> m_ToInterval;
};


//!
using ScaleTickCount = unsigned long;


//! Calculates a meaningful step width (tick) for a scale with at most \param MaxTickCount tick markers suitable for a
//! data value range comprising \RangeMinToMax.
template <typename T>
//  requires T > 0
inline double calcNiceScaleTick(T RangeMinToMax, ScaleTickCount MaxTickCount)
{
    TOO_EXPECT_THROW(MaxTickCount);
    TOO_EXPECT_THROW(RangeMinToMax > T());
    const double MaxTickCount_ = narrow_cast<double>(MaxTickCount);
    const double MinimalTick   = narrow_cast<double>(RangeMinToMax) / MaxTickCount_;
    const double magnitude = std::pow(10.0, std::floor(std::log10(MinimalTick)));
    if (too::math::almost_equal(magnitude, 0.0))
        return 0.0;
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
    TOO_EXPECT(minDataValue <= maxDataValue);
    const double minIn  = narrow_cast<double>(minDataValue);
    const double maxIn  = narrow_cast<double>(maxDataValue);
    if (too::math::almost_equal(scaleTick, 0.0))
        return std::make_pair(minIn, maxIn);
    const double minOut = std::floor(minIn / scaleTick) * scaleTick;
    const double maxOut = std::ceil(maxIn / scaleTick) * scaleTick;
    return std::make_pair(minOut, maxOut);
}

}
}

#endif
