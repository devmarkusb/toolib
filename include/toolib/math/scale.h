//! \file

#ifndef SCALE_H_ENRHFU87N83464346NG
#define SCALE_H_ENRHFU87N83464346NG

#include "../config.h"
#include "mb/ul/ul.hpp"
#include <cmath>
#include <type_traits>
#include <utility>

namespace mb::too::math {
template <typename FloatingPointType>
class MapLinearScaleIntervalToInterval {
    static_assert(std::is_floating_point<FloatingPointType>::value, "floating point type expected");

public:
    MapLinearScaleIntervalToInterval(
        const std::pair<FloatingPointType, FloatingPointType>& from_interval,
        const std::pair<FloatingPointType, FloatingPointType>& to_interval)
        : m_from_interval_(from_interval)
        , m_to_interval_(to_interval) {
        UL_EXPECT_THROW(from_interval.first < from_interval.second);
        UL_EXPECT_THROW(to_interval.first < to_interval.second);
    }

    FloatingPointType operator()(const FloatingPointType& from) const {
        return (from - m_from_interval_.first) * (m_to_interval_.second - m_to_interval_.first)
               / (m_from_interval_.second - m_from_interval_.first);
    }

    FloatingPointType inverse(const FloatingPointType& to) const {
        return to * (m_from_interval_.second - m_from_interval_.first) / (m_to_interval_.second - m_to_interval_.first)
               + m_from_interval_.first;
    }

private:
    std::pair<FloatingPointType, FloatingPointType> m_from_interval_;
    std::pair<FloatingPointType, FloatingPointType> m_to_interval_;
};

//!
using ScaleTickCount = unsigned long;

/** Calculates a meaningful step width (tick) for a scale with at most MaxTickCount tick markers suitable for a
    data value range comprising RangeMinToMax.*/
template <typename T>
//  requires T > 0
double calc_nice_scale_tick(T range_min_to_max, ScaleTickCount max_tick_count) {
    UL_EXPECT_THROW(max_tick_count);
    UL_EXPECT_THROW(range_min_to_max > T());
    const auto max_tick_count_d = ul::narrow_cast<double>(max_tick_count);
    const double minimal_tick = ul::narrow_cast<double>(range_min_to_max) / max_tick_count_d;
    const double magnitude = std::pow(10.0, std::floor(std::log10(minimal_tick)));
    if (ul::almost_equal(magnitude, 0.0))
        return 0.0;
    const double residual = minimal_tick / magnitude;
    if (residual > 5.0)
        return 10.0 * magnitude;
    else if (residual > 2.0)
        return 5.0 * magnitude;
    else if (residual > 1.0)
        return 2.0 * magnitude;
    else
        return magnitude;
}

/** Calculates a scale tick range, that contains minDataValue and maxDataValue at least, the individual
    ticks being scaleTick apart. returns a pair of <min, max> tick values (min, max being a certain integer number of
    scaleTick's apart).*/
template <typename T>
//  requires T number
std::pair<double, double> calc_scale_tick_from_to(T min_data_value, T max_data_value, double scale_tick) {
    UL_EXPECT(min_data_value <= max_data_value);
    const double min_in = ul::narrow_cast<double>(min_data_value);
    const double max_in = ul::narrow_cast<double>(max_data_value);
    if (ul::almost_equal(scale_tick, 0.0))
        return std::make_pair(min_in, max_in);
    const double min_out = std::floor(min_in / scale_tick) * scale_tick;
    const double max_out = std::ceil(max_in / scale_tick) * scale_tick;
    return std::make_pair(min_out, max_out);
}
} // namespace mb::too::math

TOO_HEADER_END

#endif
