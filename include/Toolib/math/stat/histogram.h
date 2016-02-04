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

        const auto minmax = std::minmax(std::begin(data), std::end(data));

        too::math::Map_LinearScale_Interval_to_Interval<Percent> mapData2Percent(
            std::make_pair(too::narrow_cast<Percent>(*minmax.first), too::narrow_cast<Percent>(*minmax.second)),
                    std::make_pair(zero_percent, one_hundred_percent));

        std::vector<Percent> percentual_data;

        std::for_each(std::begin(data), std::end(data), [&](const T& d)
            {
                percentual_data.push_back(mapData2Percent(too::narrow_cast<Percent>(d)));
            });

        std::sort(std::begin(percentual_data), std::end(percentual_data));

        std::array<size_t, number_of_bars> abs_rates;

        auto it = std::begin(percentual_data);
        for (BarCount i = BarCount{}; i < number_of_bars; ++i)
        {
            it = std::upper_bound(it, std::end(percentual_data), (i + 1) * 10.0);
            if (it != std::end(percentual_data))
                abs_rates[i] = std::distance(std::begin(percentual_data), it);
            else
                abs_rates[i] = 0;
        }

        const auto minmax_rates = std::minmax(std::begin(abs_rates), std::end(abs_rates));

        too::math::Map_LinearScale_Interval_to_Interval<Percent> mapAbsRates2Rel(
            std::make_pair(*minmax_rates.first, *minmax_rates.second), std::make_pair(zero_percent, one_hundred_percent));

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
