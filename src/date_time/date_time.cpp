// 2016

//! \file

#include "toolib/date_time/date_time.h"
#include "toolib/math/number.h"

namespace mb::too::date_time
{
const std::string MonthYear_decl::string_delim = "/";

void normalize::do_it(std::pair<Years, Months>& y_m)
{
    const int MonthSgn = too::math::sgn(y_m.second);
    const Months MonthAbs = std::abs(y_m.second);
    if (MonthAbs > MonthYear_decl::twelve)
    {
        UL_ASSERT(MonthSgn);
        const Years YearsCorrection = MonthSgn * (MonthAbs / MonthYear_decl::twelve);
        y_m.first += YearsCorrection;
        Months rem_months = MonthAbs % MonthYear_decl::twelve;
        if (!rem_months)
        {
            // We don't want to result in 0 months, because what should be there
            // between December and January?!
            // Although MonthYear types could be suitable for absolutes AND differences
            // normalization can respect the correct absolute format (at least
            // for input values of months != 0) since for differences it doesn't matter
            // if it's called (y=2,m=0) or (y=1,m=12), so choose the latter.
            y_m.second = MonthSgn * MonthYear_decl::twelve;
            MonthSgn > 0 ? --y_m.first : ++y_m.first;
        }
        else
            y_m.second = MonthSgn * (MonthAbs % MonthYear_decl::twelve);
    }
    const int YearSgn = too::math::sgn(y_m.first);
    if (y_m.second == 0 && y_m.first != 0)
    {
        UL_ASSERT(YearSgn);
        if (YearSgn < 0)
        {
            y_m.second = -MonthYear_decl::twelve;
            ++y_m.first;
        }
        else
        {
            y_m.second = MonthYear_decl::twelve;
            --y_m.first;
        }
    }
    if (MonthSgn == YearSgn)
        return;
    if (y_m.second < 0 && y_m.first > 0)
    {
        --y_m.first;
        y_m.second = MonthYear_decl::twelve + y_m.second;
        return;
    }
    if (y_m.second > 0 && y_m.first < 0)
    {
        ++y_m.first;
        y_m.second = -(MonthYear_decl::twelve - y_m.second);
        return;
    }
}
} // namespace mb::too::date_time
