//! \file

#ifndef DATE_TIME_H_sdhnfxeuhgf783nx4f2z3t487f
#define DATE_TIME_H_sdhnfxeuhgf783nx4f2z3t487f

#include "toolibDEF.h"
#include "../config.h"
#include "ul/warnings.h"
#include <ostream>
#include <string>
#include <utility>

namespace mb::too::date_time
{
//! Months type, represents absolute as well as difference values.
using Months = int;
//! Years type, represents absolute as well as difference values.
using Years = int;

//! Ensures month to be 1..12 and month and year having the same sign.
//! Only exception: {0, 0} will be mapped to {0, 0}.
struct TOOLIBSHARED_EXPORT normalize
{
    static void do_it(std::pair<Years, Months>& y_m);
};

//! Doesn't normalize.
struct TOOLIBSHARED_EXPORT dont_normalize
{
    static void do_it(std::pair<Years, Months>&)
    {
    }
};

//! Contract: class is always in that normalized state if normalize policy is chosen.
template <class NormalizePolicy = normalize>
class MonthYear_base;

//! Works as an absolute type as well as a difference type.
using MonthYear = MonthYear_base<>;

//! Works as a difference type only.
using MonthYear_dur = MonthYear_base<dont_normalize>;

UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4251)

struct TOOLIBSHARED_EXPORT MonthYear_decl
{
    static constexpr Months twelve = 12;
    static const std::string string_delim;
};

UL_PRAGMA_WARNINGS_POP

//! Works as an absolute type as well as a difference type.
template <class NormalizePolicy>
class MonthYear_base : public MonthYear_decl
{
public:
    MonthYear_base() = default;
    //! Params can have at least full int range.
    explicit MonthYear_base(Months m, Years y = Years());
    /** \param fromString must have form "<months><string_delim><years>",
        e.g. "2/2016", or "-387/235", full int range allowed. Please use
        MonthYear_decl::string_delim as delimiter (in the above examples defined as "/").*/
    explicit MonthYear_base(const std::string& fromString);

    //! Capable to copy from template class with different policy.
    template <class NP>
    explicit MonthYear_base(const MonthYear_base<NP>&);
    //! Capable to copy from template class with different policy.
    template <class NP>
    MonthYear_base<NormalizePolicy>& operator=(const MonthYear_base<NP>&);
    //! Capable to copy from template class with different policy.
    template <class NP>
    explicit MonthYear_base(MonthYear_base<NP>&&);
    //! Capable to copy from template class with different policy.
    template <class NP>
    MonthYear_base<NormalizePolicy>& operator=(MonthYear_base<NP>&&);

    //! Cf. suiting constructor MonthYear_base().
    MonthYear_base<NormalizePolicy>& operator=(const std::string& rhs);

    void set(Months m, Years y);
    void setMonths(Months m);
    void setYears(Years y);
    void get(Months& m, Years& y) const;
    [[nodiscard]] Months getMonths() const;
    [[nodiscard]] Years getYears() const;
    [[nodiscard]] Months asMonths() const;

    MonthYear_base<NormalizePolicy>& operator-=(const MonthYear_base<NormalizePolicy>& rhs);
    MonthYear_base<NormalizePolicy>& operator+=(const MonthYear_base<NormalizePolicy>& rhs);

    //! Outputs string in format analog to one used in constructing with MonthYear_base(const std::string& fromString),
    //! month and year delimited by MonthYear_decl::string_delim.
    template <class NP>
    friend std::ostream& operator<<(std::ostream& out, const MonthYear_base<NP>& my);

private:
    std::pair<Years, Months> y_m; // order Years, Months because of lexicographical comparison capability of std::pair
};

template <class NormalizePolicy>
MonthYear_base<NormalizePolicy> operator-(
    MonthYear_base<NormalizePolicy> lhs, const MonthYear_base<NormalizePolicy>& rhs);
template <class NormalizePolicy>
MonthYear_base<NormalizePolicy> operator+(
    MonthYear_base<NormalizePolicy> lhs, const MonthYear_base<NormalizePolicy>& rhs);

template <class NP>
bool operator==(const MonthYear_base<NP>& lhs, const MonthYear_base<NP>& rhs);
template <class NP>
bool operator!=(const MonthYear_base<NP>& lhs, const MonthYear_base<NP>& rhs);
template <class NP>
bool operator<(const MonthYear_base<NP>& lhs, const MonthYear_base<NP>& rhs);
template <class NP>
bool operator>(const MonthYear_base<NP>& lhs, const MonthYear_base<NP>& rhs);
template <class NP>
bool operator<=(const MonthYear_base<NP>& lhs, const MonthYear_base<NP>& rhs);
template <class NP>
bool operator>=(const MonthYear_base<NP>& lhs, const MonthYear_base<NP>& rhs);


const MonthYear_dur one_year(0, 1);
[[maybe_unused]] const MonthYear_dur half_year(MonthYear_decl::twelve / 2, 0);
[[maybe_unused]] const MonthYear_dur quarter_year(MonthYear_decl::twelve / 4, 0);
const MonthYear_dur one_month(1, 0);
} // namespace mb::too::date_time

//####################################################################################################################
// template implementation

#include "detail/date_time.hpp"

TOO_HEADER_END

#endif
