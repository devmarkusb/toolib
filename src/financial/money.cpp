//! \file

#include "toolib/financial/money.h"
#include "toolib/date_time/date_time.h"
#include "mb/ul/ul.hpp"
#include <locale>

namespace mb::too::fin {
//####################################################################################################################

Currency::Currency(const std::locale& loc)
    : loc_(loc) {
}

std::string Currency::get_string() const {
    if (!(this->loc_))
        return {};
    const std::string loc_enc = std::use_facet<std::moneypunct<char, true>>(*(this->loc_)).curr_symbol();
    const std::wstring utf16ws = ul::str::locenc_s2ws(loc_enc);
    return ul::str::utf16or32to8_ws2s_portable(utf16ws);
}

std::string Currency::get_symbol() const {
    if (!(this->loc_))
        return {};
    const std::string loc_enc = std::use_facet<std::moneypunct<char>>(*(this->loc_)).curr_symbol();
    const std::wstring utf16ws = ul::str::locenc_s2ws(loc_enc);
    return ul::str::utf16or32to8_ws2s_portable(utf16ws);
}

std::string Currency::get_locale_constr_name() const {
    if (!this->loc_)
        return {};
    return (*this->loc_).name();
}

bool operator==(const Currency& lhs, const Currency& rhs) {
    return lhs.loc_ == rhs.loc_;
}

bool operator!=(const Currency& lhs, const Currency& rhs) {
    return !operator==(lhs, rhs);
}

//####################################################################################################################

Money::Money(BaseType amount, const Currency& currency) {
    set(amount, currency);
}

void Money::set(BaseType amount, const Currency& currency) {
    set(amount);
    this->currency_ = currency;
}

void Money::set(BaseType amount) {
    UL_EXPECT(std::isfinite(amount));
    this->amount_ = amount;
}

Money& Money::operator=(BaseType amount) {
    this->set(amount);
    return *this;
}

auto Money::get() const -> BaseType {
    return amount_;
}

Currency Money::get_currency() const {
    return this->currency_;
}

// perhaps needs to be different for some exotic currencies?
Money::BaseType Money::get_smallest_unit(const Currency& /*unused*/) {
    return 0.01L;
}

Money::BaseType Money::get_tenth_of_smallest_unit(const Currency& /*unused*/) {
    return 0.001L;
}

Money& Money::operator-=(const Money& rhs) {
    if (this->currency_ != rhs.currency_) // otherwise not yet implemented
        throw ul::NotImplemented{"mixed currencies not yet implemented"};
    amount_ -= rhs.amount_;
    return *this;
}

Money& Money::operator+=(const Money& rhs) {
    if (this->currency_ != rhs.currency_) // otherwise not yet implemented
        throw ul::NotImplemented{"mixed currencies not yet implemented"};
    amount_ += rhs.amount_;
    return *this;
}

Money& Money::operator/=(const Money& rhs) {
    if (this->currency_ != rhs.currency_) // otherwise not yet implemented
        throw ul::NotImplemented{"mixed currencies not yet implemented"};
    amount_ /= rhs.amount_;
    return *this;
}

Money& Money::operator*=(BaseType rhs) {
    amount_ *= rhs;
    return *this;
}

Money& Money::operator/=(BaseType rhs) {
    // Don't check for div by zero, since the floating point result will just be inf (fine).
    // You then would have to check for multiplication by inf as well.
    // At least the checks shouldn't be done here, since the function just perfect forwards the
    // mathematical operation.
    amount_ /= rhs;
    return *this;
}

Money Money::operator-() const {
    Money tmp{*this};
    tmp.amount_ = -tmp.amount_;
    return tmp;
}

Money operator-(Money lhs, const Money& rhs) {
    lhs -= rhs;
    return lhs;
}

Money operator+(Money lhs, const Money& rhs) {
    lhs += rhs;
    return lhs;
}

Money operator/(Money lhs, const Money& rhs) {
    lhs /= rhs;
    return lhs;
}

Money operator/(Money lhs, const Money::BaseType& rhs) {
    lhs /= rhs;
    return lhs;
}

Money operator*(Money lhs, const Money::BaseType& rhs) {
    lhs *= rhs;
    return lhs;
}

Money operator*(Money::BaseType lhs, const Money& rhs) {
    Money tmp(rhs);
    tmp *= lhs;
    return tmp;
}

bool operator==(const Money& lhs, const Money& rhs) {
    if (lhs.currency_ != rhs.currency_) // otherwise not yet implemented
        throw ul::NotImplemented{"mixed currencies not yet implemented"};
    return ul::almost_equal(lhs.amount_, rhs.amount_);
}

bool operator<(const Money& lhs, const Money& rhs) {
    if (lhs.currency_ != rhs.currency_) // otherwise not yet implemented
        throw ul::NotImplemented{"mixed currencies not yet implemented"};
    return lhs.amount_ < rhs.amount_;
}

bool operator!=(const Money& lhs, const Money& rhs) {
    return !operator==(lhs, rhs);
}

bool operator>(const Money& lhs, const Money& rhs) {
    return operator<(rhs, lhs);
}

bool operator<=(const Money& lhs, const Money& rhs) {
    return operator==(lhs, rhs) || !operator>(lhs, rhs);
}

bool operator>=(const Money& lhs, const Money& rhs) {
    return operator==(lhs, rhs) || !operator<(lhs, rhs);
}

bool equal_sufficiently(const Money& lhs, const Money& rhs) {
    if (lhs.currency_ != rhs.currency_) // otherwise not yet implemented
        throw ul::NotImplemented{"mixed currencies not yet implemented"};
    // tenth of smallest unit needed since this is the relevant digit for rounding
    return ul::math::approx_equal(lhs.amount_, rhs.amount_, Money::get_tenth_of_smallest_unit());
}

//####################################################################################################################

Fraction InterestPa::yearly_effective_to_monthly_relative(Fraction pa) {
    return std::pow(1.0L + pa, 1.0L / too::date_time::MonthYearDecl::twelve) - 1.0L;
}
} // namespace mb::too::fin
