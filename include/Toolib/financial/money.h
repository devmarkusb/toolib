// Markus Borris, 2016
// This file is part of Toolib library. Open source.

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

//!
class TOOLIBSHARED_EXPORT Currency
{
public:
    //! The default of \param currency means using the user preferred locale.
    /** On e.g. a German system this could be std::locale("de_DE.utf8") internally.*/
    explicit Currency(const std::locale& loc = std::locale(""));

    std::string getString() const;
    std::string getSymbol() const;
    std::string getLocaleConstrName() const;

    friend bool operator==(const Currency& lhs, const Currency& rhs);
    friend bool operator!=(const Currency& lhs, const Currency& rhs);

private:
    std::locale loc;
};

bool operator==(const Currency& lhs, const Currency& rhs);
bool operator!=(const Currency& lhs, const Currency& rhs);


//##########################################################################################################

//!
class TOOLIBSHARED_EXPORT Money
{
public:
    using BaseType = long double;

    //! The default of \param currency means using the user preferred locale's currency.
    /** On e.g. a German system this could be EUR internally.*/
    explicit Money(BaseType amount = BaseType(), const Currency& currency = Currency());

    void set(BaseType amount, const Currency& currency);

    BaseType get() const;
    Currency getCurrency() const;

    Money& operator-=(const Money& rhs);
    Money& operator+=(const Money& rhs);
    Money& operator*=(BaseType rhs);
    Money& operator/=(BaseType rhs);

    friend bool operator==(const Money& lhs, const Money& rhs);
    friend bool operator!=(const Money& lhs, const Money& rhs);
    friend bool operator<(const Money& lhs, const Money& rhs);
    friend bool operator>(const Money& lhs, const Money& rhs);
    friend bool operator<=(const Money& lhs, const Money& rhs);
    friend bool operator>=(const Money& lhs, const Money& rhs);

private:
    BaseType amount = BaseType();
    Currency currency;
};

TOOLIBSHARED_EXPORT Money operator-(Money lhs, const Money& rhs);
TOOLIBSHARED_EXPORT Money operator+(Money lhs, const Money& rhs);

TOOLIBSHARED_EXPORT bool operator==(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator!=(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator<(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator>(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator<=(const Money& lhs, const Money& rhs);
TOOLIBSHARED_EXPORT bool operator>=(const Money& lhs, const Money& rhs);



}
}


#endif
