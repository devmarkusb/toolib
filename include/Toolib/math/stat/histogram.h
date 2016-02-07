// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef HISTOGRAM_H_idungz8c7457gx812334gbxxxdesg
#define HISTOGRAM_H_idungz8c7457gx812334gbxxxdesg

#include <vector>
#include <algorithm>
#include <array>
#include "Toolib/math/scale.h"
#include "Toolib/math/percent.h"
#include "Toolib/narrow.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/math/floating_point.h"


namespace too
{

namespace stat
{

using too::math::Percent;

//! Assume you have some arbitrary 1D data set. This class helps transforming that into
//! a 'percent over percent' data set.
class Histogram_percentual
{
public:
    using BarCount                       = unsigned int;
    static const BarCount number_of_bars = 10;

    template <typename T>
    // requires T castable to Percent type
    Histogram_percentual(const std::vector<T>& data)
    {
        using too::math::zero_percent;
        using too::math::one_hundred_percent;

        if (data.empty())
            return;

        const auto minmax = std::minmax_element(std::begin(data), std::end(data));

        too::math::Map_LinearScale_Interval_to_Interval<Percent> mapData2Percent(
            std::make_pair(Percent(), too::narrow_cast<Percent>(*minmax.second)),
                    std::make_pair(zero_percent, one_hundred_percent));

        std::vector<Percent> percentual_data;

        std::for_each(std::begin(data), std::end(data), [&](const T& d)
            {
                percentual_data.push_back(mapData2Percent(too::narrow_cast<Percent>(d)));
            });

        std::sort(std::begin(percentual_data), std::end(percentual_data));

        std::array<size_t, number_of_bars> abs_rates;
        size_t sum_of_rates = 0;

        auto prev_it = std::begin(percentual_data);
        auto it = prev_it;
        for (BarCount i = BarCount{}; i < number_of_bars; ++i)
        {
            it = std::upper_bound(prev_it, std::end(percentual_data), (i + 1) * 10.0);
            if (it != std::end(percentual_data) && it != prev_it)
            {
                abs_rates[i] = std::distance(prev_it, it);
                sum_of_rates += abs_rates[i];
            }
            else
                abs_rates[i] = 0;
            prev_it = it;
        }
        // 100% data entries aren't counted in the previous loop, only smaller ones;
        // already tried lower_bound and almost<=
        for (auto it = percentual_data.rbegin(); it != percentual_data.rend(); ++it)
        {
            if (too::math::almost_equal(*it, too::math::one_hundred_percent))
            {
                ++abs_rates[abs_rates.size() - 1];
                ++sum_of_rates;
            }
            else
                break;
        }

        const auto minmax_rates = std::minmax_element(std::begin(abs_rates), std::end(abs_rates));

        too::math::Map_LinearScale_Interval_to_Interval<Percent> mapAbsRates2Rel(
            std::make_pair(Percent(), sum_of_rates), std::make_pair(zero_percent, one_hundred_percent));

        std::transform(std::begin(abs_rates), std::end(abs_rates), std::begin(this->bars), [&](size_t ar)
            {
                return mapAbsRates2Rel(ar);
            });
    }

    const std::array<Percent, number_of_bars>& getBars() const { return this->bars; }

private:
    std::array<Percent, number_of_bars> bars
#if TOO_HAS_BRACE_INIT_MEMBER_NON_STATIC
    {{}}
#endif
    ;
};

}

}

#endif
