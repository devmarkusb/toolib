//! \file

#ifndef DATE_TIME_H_SDHNFXEUHGF783NX4F2Z3T487F
#define DATE_TIME_H_SDHNFXEUHGF783NX4F2Z3T487F

#include "toolibDEF.h"
#include "../config.h"
#include "ul/warnings.h"
#include <ostream>
#include <string>
#include <utility>

namespace mb::too::date_time {
//! Months type, represents absolute as well as difference values.
using Months = int;
//! Years type, represents absolute as well as difference values.
using Years = int;

//! Ensures month to be 1..12 and month and year having the same sign. Only exception: {0, 0} will be mapped to {0, 0}.
struct TOOLIBSHARED_EXPORT Normalize {
    static constexpr void do_it(std::pair<Years, Months>& y_m);
};

//! Doesn't normalize.
struct TOOLIBSHARED_EXPORT DontNormalize {
    static constexpr void do_it(std::pair<Years, Months>&) {
    }
};

//! Contract: class is always in that normalized state if normalize policy is chosen.
template <class NormalizePolicy = Normalize>
class MonthYearBase;

//! Works as an absolute type as well as a difference type.
using MonthYear = MonthYearBase<>;

//! Works as a difference type only.
using MonthYearDur = MonthYearBase<DontNormalize>;

UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4251)

struct TOOLIBSHARED_EXPORT MonthYearDecl {
    static constexpr Months twelve = 12;
    static const std::string string_delim;
};

UL_PRAGMA_WARNINGS_POP

//! Works as an absolute type as well as a difference type.
template <class NormalizePolicy>
class MonthYearBase : public MonthYearDecl {
public:
    MonthYearBase() = default;

    //! Params can have at least full int range.
    constexpr explicit MonthYearBase(Months m, Years y = Years())
        : y_m_{y, m} {
        NormalizePolicy::do_it(this->y_m_);
    }

    /** \param from_string must have form "<months><string_delim><years>",
        e.g. "2/2016", or "-387/235", full int range allowed. Please use
        MonthYearDecl::string_delim as delimiter (in the above examples defined as "/").*/
    explicit MonthYearBase(const std::string& from_string);

    //! Capable to copy from template class with different policy.
    template <class NP>
    explicit MonthYearBase(const MonthYearBase<NP>&);
    //! Capable to copy from template class with different policy.
    template <class NP>
    MonthYearBase<NormalizePolicy>& operator=(const MonthYearBase<NP>&);
    //! Capable to copy from template class with different policy.
    template <class NP>
    explicit MonthYearBase(MonthYearBase<NP>&&);
    //! Capable to copy from template class with different policy.
    template <class NP>
    MonthYearBase<NormalizePolicy>& operator=(MonthYearBase<NP>&&);

    //! Cf. suiting constructor MonthYearBase().
    MonthYearBase<NormalizePolicy>& operator=(const std::string& rhs);

    void set(Months m, Years y);
    void set_months(Months m);
    void set_years(Years y);
    void get(Months& m, Years& y) const;
    [[nodiscard]] Months get_months() const;
    [[nodiscard]] Years get_years() const;
    [[nodiscard]] Months as_months() const;

    MonthYearBase<NormalizePolicy>& operator-=(const MonthYearBase<NormalizePolicy>& rhs);
    MonthYearBase<NormalizePolicy>& operator+=(const MonthYearBase<NormalizePolicy>& rhs);

    /** Outputs string in format analog to one used in constructing with MonthYearBase(const std::string& from_string),
        month and year delimited by MonthYearDecl::string_delim.*/
    template <class NP>
    friend std::ostream& operator<<(std::ostream& out, const MonthYearBase<NP>& my);

private:
    std::pair<Years, Months> y_m_; // order Years, Months because of lexicographical comparison capability of std::pair
};

template <class NormalizePolicy>
MonthYearBase<NormalizePolicy> operator-(MonthYearBase<NormalizePolicy> lhs, const MonthYearBase<NormalizePolicy>& rhs);
template <class NormalizePolicy>
MonthYearBase<NormalizePolicy> operator+(MonthYearBase<NormalizePolicy> lhs, const MonthYearBase<NormalizePolicy>& rhs);

template <class NP>
bool operator==(const MonthYearBase<NP>& lhs, const MonthYearBase<NP>& rhs);
template <class NP>
bool operator!=(const MonthYearBase<NP>& lhs, const MonthYearBase<NP>& rhs);
template <class NP>
bool operator<(const MonthYearBase<NP>& lhs, const MonthYearBase<NP>& rhs);
template <class NP>
bool operator>(const MonthYearBase<NP>& lhs, const MonthYearBase<NP>& rhs);
template <class NP>
bool operator<=(const MonthYearBase<NP>& lhs, const MonthYearBase<NP>& rhs);
template <class NP>
bool operator>=(const MonthYearBase<NP>& lhs, const MonthYearBase<NP>& rhs);


constexpr MonthYearDur one_year(0, 1);
[[maybe_unused]] constexpr MonthYearDur half_year(MonthYearDecl::twelve / 2, 0);
[[maybe_unused]] constexpr MonthYearDur quarter_year(MonthYearDecl::twelve / 4, 0);
constexpr MonthYearDur one_month(1, 0);
} // namespace mb::too::date_time

//####################################################################################################################
// template implementation

#include "detail/date_time.hpp"

TOO_HEADER_END

#endif
