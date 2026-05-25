//! \file

#ifndef MONEY_HPP_WEZTUIOLKJHGF34567
#define MONEY_HPP_WEZTUIOLKJHGF34567

#include "../config.hpp"
#include "mb/ul/ul.hpp"
#include <locale>
#include <optional>
#include <string>

namespace mb::too::fin {
//####################################################################################################################

UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4251)

//!
class Currency {
public:
    //! Setting loc to std::locale("") means using the user preferred locale.
    /** On e.g. a German system this could be std::locale("de_DE.utf8") internally.*/
    explicit Currency(const std::locale& loc);
    /** Per default constructs a non-currency, in case you want to deal with currency-less amounts of money for
        convenience.*/
    Currency() = default;

    [[nodiscard]] std::string get_string() const;
    [[nodiscard]] std::string get_symbol() const;
    //! \return empty string if no locale was used to construct.
    [[nodiscard]] std::string get_locale_constr_name() const;

    friend bool operator==(const Currency& lhs, const Currency& rhs);
    friend bool operator!=(const Currency& lhs, const Currency& rhs);

private:
    std::optional<std::locale> loc_;
};

UL_PRAGMA_WARNINGS_POP

bool operator==(const Currency& lhs, const Currency& rhs);
bool operator!=(const Currency& lhs, const Currency& rhs);

//####################################################################################################################

//!
class Money {
public:
    using BaseType = long double;

    /** The default of currency means no currency at all. You can get the user preferred locale's currency by passing
        Currency{}. On e.g. a German system this could be EUR internally.*/
    explicit Money(BaseType amount = {}, const Currency& currency = Currency{});

    void set(BaseType amount, const Currency& currency);
    //! Doesn't change currency.
    void set(BaseType amount);
    //! Like set(BaseType).
    Money& operator=(BaseType amount);

    [[nodiscard]] BaseType get() const;
    [[nodiscard]] Currency get_currency() const;

    static BaseType get_smallest_unit(const Currency& currency = Currency{});
    static BaseType get_tenth_of_smallest_unit(const Currency& currency = Currency{});

    Money& operator-=(const Money& rhs);
    Money& operator+=(const Money& rhs);
    Money& operator/=(const Money& rhs);
    Money& operator*=(BaseType rhs);
    Money& operator/=(BaseType rhs);
    Money operator-() const;

    friend bool operator==(const Money& lhs, const Money& rhs);
    friend bool operator!=(const Money& lhs, const Money& rhs);
    friend bool operator<(const Money& lhs, const Money& rhs);
    friend bool operator>(const Money& lhs, const Money& rhs);
    friend bool operator<=(const Money& lhs, const Money& rhs);
    friend bool operator>=(const Money& lhs, const Money& rhs);
    friend bool equal_sufficiently(const Money& lhs, const Money& rhs);

private:
    BaseType amount_{};
    Currency currency_;
};

Money operator-(Money lhs, const Money& rhs);
Money operator+(Money lhs, const Money& rhs);
Money operator/(Money lhs, const Money& rhs);
Money operator/(Money lhs, const Money::BaseType& rhs);
Money operator*(Money lhs, const Money::BaseType& rhs);
Money operator*(Money::BaseType lhs, const Money& rhs);

bool operator==(const Money& lhs, const Money& rhs);
bool operator!=(const Money& lhs, const Money& rhs);
bool operator<(const Money& lhs, const Money& rhs);
bool operator>(const Money& lhs, const Money& rhs);
bool operator<=(const Money& lhs, const Money& rhs);
bool operator>=(const Money& lhs, const Money& rhs);

//! 'equal to the cent'; to be used for end result checks, not so much within calculations
bool equal_sufficiently(const Money& lhs, const Money& rhs);


//####################################################################################################################

//! Should be understood in relation to Fraction as follows: 'Percent' == 'Fraction' * 100.
using Percent = long double;
//! Should be understood in relation to Percent as follows: 'Percent' == 'Fraction' * 100.
using Fraction = long double;

class InterestPa {
public:
    explicit InterestPa(Percent p = Percent())
        : p_(p) {
    }

    InterestPa& operator=(Percent p) {
        this->p_ = p;
        return *this;
    }

    /*implicit*/ operator Percent() const {
        return this->p_;
    }

    [[nodiscard]] Fraction divided_by100() const {
        return this->p_ / 100.0l;
    }

    static Fraction yearly_effective_to_monthly_relative(Fraction pa);

    template <class Archive>
    void serialize(Archive& archive) {
        archive(this->p_);
    }

private:
    Percent p_ = Percent();
};
} // namespace mb::too::fin


#endif
