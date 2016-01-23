// Markus Borris, 2016
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file

#include "Toolib/financial/money.h"
#include <locale>
#include "Toolib/assert.h"
#include "Toolib/error.h"


namespace too
{
namespace fin
{

//##########################################################################################################

Currency::Currency(const std::locale& loc) : loc(loc) {}

std::string Currency::getString() const
{
    return std::use_facet<std::moneypunct<char, true>>(this->loc).curr_symbol();
}

std::string Currency::getSymbol() const
{
    return std::use_facet<std::moneypunct<char>>(this->loc).curr_symbol();
}

std::string Currency::getLocaleConstrName() const
{
    return this->loc.name();
}

bool operator==(const Currency& lhs, const Currency& rhs)
{
    return lhs.loc == rhs.loc;
}

bool operator!=(const Currency& lhs, const Currency& rhs)
{
    return !operator==(lhs, rhs);
}


//##########################################################################################################

Money::Money(BaseType amount, const Currency& currency)
{
    set(amount, currency);
}

void Money::set(BaseType amount, const Currency& currency)
{
    this->amount = amount;
    this->currency = currency;
}

auto Money::get() const -> BaseType
{
    return amount;
}

Currency Money::getCurrency() const
{
    return this->currency;
}

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

bool operator==(const Money& lhs, const Money& rhs)
{
    if (lhs.currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    return lhs.amount == rhs.amount;
}

bool operator<(const Money& lhs, const Money& rhs)
{
    if (lhs.currency != rhs.currency) // otherwise not yet implemented
        throw too::not_implemented("mixed currencies not yet implemented");
    return lhs.amount < rhs.amount;
}

bool operator!=(const Money& lhs, const Money& rhs)
{
    return !operator==(lhs, rhs);
}

bool operator>(const Money& lhs, const Money& rhs)
{
    return operator<(rhs, lhs);
}

bool operator<=(const Money& lhs, const Money& rhs)
{
    return !operator>(lhs, rhs);
}

bool operator>=(const Money& lhs, const Money& rhs)
{
    return !operator<(lhs, rhs);
}

}
}
