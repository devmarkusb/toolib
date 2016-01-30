// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef MONEY_H_weztuiolkjhgf34567
#define MONEY_H_weztuiolkjhgf34567

#include <string>
#include <locale>
#include "Toolib/optional.h"
#include "ToolibDEF.h"


namespace too
{
namespace fin
{

//##########################################################################################################

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4251_BEGIN"
//!
class TOOLIBSHARED_EXPORT Currency
{
public:
    struct None_t {};
    struct err_constructed_empty : std::exception {};

    //! The default of \param currency means using the user preferred locale.
    /** On e.g. a German system this could be std::locale("de_DE.utf8") internally.*/
    explicit Currency(const std::locale& loc = std::locale(""));
    //! Constructs a non-currency, in case you want to deal with currency less amounts of money for convenience.
    //! Just pass too::fin::none as parameter.
    Currency(None_t);

    std::string getString() const;
    std::string getSymbol() const;
    //! Throws Currency::err_constructed_empty if no locale was used to construct.
    std::string getLocaleConstrName() const;

    friend bool operator==(const Currency& lhs, const Currency& rhs);
    friend bool operator!=(const Currency& lhs, const Currency& rhs);

private:
    too::opt<std::locale> loc;
};
const Currency::None_t none;
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"

bool operator==(const Currency& lhs, const Currency& rhs);
bool operator!=(const Currency& lhs, const Currency& rhs);


//##########################################################################################################

//!
class TOOLIBSHARED_EXPORT Money
{
public:
    using BaseType = long double;

    //! The default of \param currency means no currency at all.
    //! You can get the user preferred locale's currency by passing Currency().
    /** On e.g. a German system this could be EUR internally.*/
    explicit Money(BaseType amount = BaseType(), const Currency& currency = Currency(none));

    void set(BaseType amount, const Currency& currency);
    //! Doesn't change currency.
    void set(BaseType amount);
    //! Like set(BaseType).
    Money& operator=(BaseType amount);

    BaseType get() const;
    Currency getCurrency() const;

    Money& operator-=(const Money& rhs);
    Money& operator+=(const Money& rhs);
    Money& operator/=(const Money& rhs);
    Money& operator*=(BaseType rhs);
    Money& operator/=(BaseType rhs);

    friend TOOLIBSHARED_EXPORT bool operator==(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator!=(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator<(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator>(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator<=(const Money& lhs, const Money& rhs);
    friend TOOLIBSHARED_EXPORT bool operator>=(const Money& lhs, const Money& rhs);

private:
    BaseType amount = BaseType();
    Currency currency;
};

TOOLIBSHARED_EXPORT Money operator-(Money lhs, const Money& rhs);
TOOLIBSHARED_EXPORT Money operator+(Money lhs, const Money& rhs);
TOOLIBSHARED_EXPORT Money operator/(Money lhs, const Money& rhs);

TOOLIBSHARED_EXPORT bool operator==(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator!=(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator<(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator>(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator<=(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator>=(const Money& lhs, const Money& rhs);


//##########################################################################################################

//! Should be understood in relation to Fraction as follows: 'Percent' == 'Fraction' * 100.
using Percent = long double;
//! Should be understood in relation to Percent as follows: 'Percent' == 'Fraction' * 100.
using Fraction = long double;

class TOOLIBSHARED_EXPORT Interest_pa
{
public:
    explicit Interest_pa(Percent p = Percent()) : p(p) {}

    void operator=(Percent p) { this->p = p; }

    operator Percent() const { return this->p; }

    Fraction dividedBy100() const { return this->p / 100.0; }

    static Fraction YearlyEffective_to_MonthlyRelative(Fraction pa);

private:
    Percent p = Percent();
};

}
}


#endif
