// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#include "Toolib/financial/money.h"
#include <locale>
#include "Toolib/assert.h"
#include "Toolib/error.h"
#include "Toolib/date_time/date_time.h"
#include "Toolib/string/str_convert.h"
#include "Toolib/math/floating_point.h"


namespace too
{
namespace fin
{

//##########################################################################################################

Currency::Currency(const std::locale& loc) : loc(loc) {}

std::string Currency::getString() const
{
    if (!(this->loc))
        return std::string();
    const std::string loc_enc  = std::use_facet<std::moneypunct<char, true>>(*(this->loc)).curr_symbol();
    const std::wstring utf16ws = too::str::locenc_s2ws(loc_enc);
    return too::str::utf16to8_ws2s_portable(utf16ws);
}

std::string Currency::getSymbol() const
{
    if (!(this->loc))
        return std::string();
    const std::string loc_enc  = std::use_facet<std::moneypunct<char>>(*(this->loc)).curr_symbol();
    const std::wstring utf16ws = too::str::locenc_s2ws(loc_enc);
    return too::str::utf16to8_ws2s_portable(utf16ws);
}

std::string Currency::getLocaleConstrName() const
{
    if (!this->loc)
        return {};
    return (*this->loc).name();
}

bool operator==(const Currency& lhs, const Currency& rhs) { return lhs.loc == rhs.loc; }

bool operator!=(const Currency& lhs, const Currency& rhs) { return !operator==(lhs, rhs); }


//##########################################################################################################

Money::Money(BaseType amount, const Currency& currency) { set(amount, currency); }

void Money::set(BaseType amount, const Currency& currency)
{
    set(amount);
    this->currency = currency;
}

void Money::set(BaseType amount)
{
    TOO_EXPECT(std::isfinite(amount));
    this->amount = amount;
}

Money& Money::operator=(BaseType amount)
{
    this->set(amount);
    return *this;
}

auto Money::get() const -> BaseType { return amount; }

Currency Money::getCurrency() const { return this->currency; }

// perhaps needs to be different for some exotic currencies?
Money::BaseType Money::getSmallestUnit(const Currency&) { return 0.01L; }
Money::BaseType Money::getTenthOfSmallestUnit(const Currency&) { return 0.001L; }

Money& Money::operator-=(const Money& rhs)
{
    if (this->currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    amount -= rhs.amount;
    return *this;
}

Money& Money::operator+=(const Money& rhs)
{
    if (this->currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    amount += rhs.amount;
    return *this;
}

Money& Money::operator/=(const Money& rhs)
{
    if (this->currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    amount /= rhs.amount;
    return *this;
}

Money& Money::operator*=(BaseType rhs)
{
    amount *= rhs;
    return *this;
}

Money& Money::operator/=(BaseType rhs)
{
    // Don't check for div by zero, since the floating point result will just be inf (fine).
    // You then would have to check for multiplication by inf as well.
    // At least the checks shouldn't be done here, since the function just perfect forwards the
    // mathematical operation.
    amount /= rhs;
    return *this;
}

Money Money::operator-() const
{
    Money tmp{*this};
    tmp.amount = -tmp.amount;
    return tmp;
}

Money operator-(Money lhs, const Money& rhs)
{
    lhs -= rhs;
    return lhs;
}

Money operator+(Money lhs, const Money& rhs)
{
    lhs += rhs;
    return lhs;
}

Money operator/(Money lhs, const Money& rhs)
{
    lhs /= rhs;
    return lhs;
}

Money operator/(Money lhs, const Money::BaseType& rhs)
{
    lhs /= rhs;
    return lhs;
}

Money operator*(Money lhs, const Money::BaseType& rhs)
{
    lhs *= rhs;
    return lhs;
}

Money operator*(Money::BaseType lhs, const Money& rhs)
{
    Money tmp(rhs);
    tmp *= lhs;
    return tmp;
}

bool operator==(const Money& lhs, const Money& rhs)
{
    if (lhs.currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    return too::math::almost_equal(lhs.amount, rhs.amount);
}

bool operator<(const Money& lhs, const Money& rhs)
{
    if (lhs.currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    return lhs.amount < rhs.amount;
}

bool operator!=(const Money& lhs, const Money& rhs) { return !operator==(lhs, rhs); }

bool operator>(const Money& lhs, const Money& rhs) { return operator<(rhs, lhs); }

bool operator<=(const Money& lhs, const Money& rhs)
{
    return operator==(lhs, rhs) || !operator>(lhs, rhs);
}

bool operator>=(const Money& lhs, const Money& rhs)
{
    return operator==(lhs, rhs) || !operator<(lhs, rhs);
}

bool equal_sufficiently(const Money& lhs, const Money& rhs)
{
    if (lhs.currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    // tenth of smallest unit needed since this is the relevant digit for rounding
    return too::math::approx_equal(lhs.amount, rhs.amount, Money::getTenthOfSmallestUnit());
}


//##########################################################################################################


Fraction Interest_pa::YearlyEffective_to_MonthlyRelative(Fraction pa)
{
    return std::pow(1.0L + pa, 1.0L / too::date_time::MonthYear_decl::twelve) - 1.0L;
}
}
}
