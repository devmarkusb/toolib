//! \file

#include "toolib/date_time/date_time.h"
#include "ul/ul.h"

namespace mb::too::date_time {
const std::string MonthYearDecl::string_delim = "/";

constexpr void Normalize::do_it(std::pair<Years, Months>& y_m) {
    const int month_sgn = ul::math::sgn(y_m.second);
    const Months month_abs = std::abs(y_m.second);
    if (month_abs > MonthYearDecl::twelve) {
        UL_ASSERT(month_sgn);
        const Years years_correction = month_sgn * (month_abs / MonthYearDecl::twelve);
        y_m.first += years_correction;
        const Months rem_months = month_abs % MonthYearDecl::twelve;
        if (!rem_months) {
            // We don't want to result in 0 months, because what should be there
            // between December and January?!
            // Although MonthYear types could be suitable for absolutes AND differences
            // normalization can respect the correct absolute format (at least
            // for input values of months != 0) since for differences it doesn't matter
            // if it's called (y=2,m=0) or (y=1,m=12), so choose the latter.
            y_m.second = month_sgn * MonthYearDecl::twelve;
            month_sgn > 0 ? --y_m.first : ++y_m.first;
        } else
            y_m.second = month_sgn * (month_abs % MonthYearDecl::twelve);
    }
    const int year_sgn = ul::math::sgn(y_m.first);
    if (y_m.second == 0 && y_m.first != 0) {
        UL_ASSERT(year_sgn);
        if (year_sgn < 0) {
            y_m.second = -MonthYearDecl::twelve;
            ++y_m.first;
        } else {
            y_m.second = MonthYearDecl::twelve;
            --y_m.first;
        }
    }
    if (month_sgn == year_sgn)
        return;
    if (y_m.second < 0 && y_m.first > 0) {
        --y_m.first;
        y_m.second = MonthYearDecl::twelve + y_m.second;
        return;
    }
    if (y_m.second > 0 && y_m.first < 0) {
        ++y_m.first;
        y_m.second = -(MonthYearDecl::twelve - y_m.second);
        return;
    }
}
} // namespace mb::too::date_time
