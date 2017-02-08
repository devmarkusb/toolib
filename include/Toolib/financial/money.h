// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef MONEY_H_weztuiolkjhgf34567
#define MONEY_H_weztuiolkjhgf34567

#include "ToolibDEF.h"
#include "Toolib/optional.h"
#include <locale>
#include <string>


namespace too
{
namespace fin
{

//####################################################################################################################

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4251_BEGIN"
//!
class TOOLIBSHARED_EXPORT Currency
{
public:
    //! Setting \param loc to std::locale("") means using the user preferred locale.
    /** On e.g. a German system this could be std::locale("de_DE.utf8") internally.*/
    explicit Currency(const std::locale& loc);
    //! Per default constructs a non-currency, in case you want to deal with currency-less amounts of money for
    //! convenience.
    Currency() = default;

    std::string getString() const;
    std::string getSymbol() const;
    //! \returns empty string if no locale was used to construct.
    std::string getLocaleConstrName() const;

    friend bool operator==(const Currency& lhs, const Currency& rhs);
    friend bool operator!=(const Currency& lhs, const Currency& rhs);

private:
    too::opt<std::locale> loc;
};
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"

bool operator==(const Currency& lhs, const Currency& rhs);
bool operator!=(const Currency& lhs, const Currency& rhs);


//####################################################################################################################

//!
class TOOLIBSHARED_EXPORT Money
{
public:
    using BaseType = long double;

    //! The default of \param currency means no currency at all.
    //! You can get the user preferred locale's currency by passing Currency().
    /** On e.g. a German system this could be EUR internally.*/
    explicit Money(BaseType amount = {}, const Currency& currency = Currency{});

    void set(BaseType amount, const Currency& currency);
    //! Doesn't change currency.
    void set(BaseType amount);
    //! Like set(BaseType).
    Money& operator=(BaseType amount);

    BaseType get() const;
    Currency getCurrency() const;

    static BaseType getSmallestUnit(const Currency& currency = Currency{});
    static BaseType getTenthOfSmallestUnit(const Currency& currency = Currency{});

    Money& operator-=(const Money& rhs);
    Money& operator+=(const Money& rhs);
    Money& operator/=(const Money& rhs);
    Money& operator*=(BaseType rhs);
    Money& operator/=(BaseType rhs);
    Money operator-() const;

    friend TOOLIBSHARED_EXPORT bool operator==(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator!=(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator<(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator>(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator<=(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator>=(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool equal_sufficiently(const Money& lhs, const Money& rhs);

private:
    BaseType amount{};
    Currency currency;
};

TOOLIBSHARED_EXPORT Money operator-(Money lhs, const Money& rhs);
TOOLIBSHARED_EXPORT Money operator+(Money lhs, const Money& rhs);
TOOLIBSHARED_EXPORT Money operator/(Money lhs, const Money& rhs);
TOOLIBSHARED_EXPORT Money operator/(Money lhs, const Money::BaseType& rhs);
TOOLIBSHARED_EXPORT Money operator*(Money lhs, const Money::BaseType& rhs);
TOOLIBSHARED_EXPORT Money operator*(Money::BaseType lhs, const Money& rhs);

TOOLIBSHARED_EXPORT bool operator==(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator!=(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator<(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator>(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator<=(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator>=(const Money& lhs, const Money& rhs);

//! 'equal to the cent'; to be used for end result checks, not so much within calculations
TOOLIBSHARED_EXPORT bool equal_sufficiently(const Money& lhs, const Money& rhs);


//####################################################################################################################

//! Should be understood in relation to Fraction as follows: 'Percent' == 'Fraction' * 100.
using Percent = long double;
//! Should be understood in relation to Percent as follows: 'Percent' == 'Fraction' * 100.
using Fraction = long double;

class TOOLIBSHARED_EXPORT Interest_pa
{
public:
    explicit Interest_pa(Percent p = Percent()) : p(p) {}

    void operator=(Percent p_) { this->p = p_; }

    operator Percent() const { return this->p; }

    Fraction dividedBy100() const { return this->p / 100.0; }

    static Fraction YearlyEffective_to_MonthlyRelative(Fraction pa);

    template <class Archive>
    void serialize(Archive& archive)
    {
        archive(this->p);
    }

private:
    Percent p = Percent();
};
}
}


#endif
