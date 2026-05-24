//! \file

#ifndef QUANTITY_UNIT_H_DFHGXUNGH38HGXT38GB
#define QUANTITY_UNIT_H_DFHGXUNGH38HGXT38GB

#include "../config.h"
#include "mb/ul/ul.hpp"
#include <exception>
#include <map>
#include <ratio>
#include <string>
#include <type_traits>
#include <vector>

namespace mb::too::math {
struct RationalLess {
    bool operator()(const ul::math::Rational& lhs, const ul::math::Rational& rhs) const {
        return lhs.as_floating_point<long double>() < rhs.as_floating_point<long double>();
    }
};

using MapRationalString = std::map<ul::math::Rational, std::string, RationalLess>;

//! To create a simple default for constructing a Unit in cases you don't want to think about details.
inline MapRationalString create_map_ratio_simple(const std::string& base_unit_name = {}) {
    MapRationalString ret;
    ret[ul::math::one] = std::string{ul::math::one_symb} + base_unit_name;
    return ret;
}

//! This is for all quantities that work with standard SI unit prefixes.
//! Unfortunately you will have to build custom maps for some quantity types, e.g. time, money,
//! even for masses you would like to write t instead of Mg; also internationalization plays a role
//! so that it is better to keep strings under your control.
/** \param only_factors_of_thousand resticts output to the most popular steps of thousands, factors of 10^3.
    That misses e.g. cm, dm.*/
inline MapRationalString create_map_ratio_si_prefix_unitname(
    const std::string& base_unit_name, bool only_factors_of_thousand = true) {
    MapRationalString ret;

    ret[ul::math::atto] = std::string{ul::math::atto_symb} + base_unit_name;
    ret[ul::math::femto] = std::string{ul::math::femto_symb} + base_unit_name;
    ret[ul::math::pico] = std::string{ul::math::pico_symb} + base_unit_name;
    ret[ul::math::nano] = std::string{ul::math::nano_symb} + base_unit_name;
    ret[ul::math::micro] = std::string{ul::math::micro_symb} + base_unit_name;
    ret[ul::math::milli] = std::string{ul::math::milli_symb} + base_unit_name;
    if (!only_factors_of_thousand) {
        ret[ul::math::centi] = std::string{ul::math::centi_symb} + base_unit_name;
        ret[ul::math::deci] = std::string{ul::math::deci_symb} + base_unit_name;
    }

    ret[ul::math::one] = std::string{ul::math::one_symb} + base_unit_name;

    if (!only_factors_of_thousand) {
        ret[ul::math::deca] = std::string{ul::math::deka_symb} + base_unit_name;
        ret[ul::math::hecto] = std::string{ul::math::hecto_symb} + base_unit_name;
    }
    ret[ul::math::kilo] = std::string{ul::math::kilo_symb} + base_unit_name;
    ret[ul::math::mega] = std::string{ul::math::mega_symb} + base_unit_name;
    ret[ul::math::giga] = std::string{ul::math::giga_symb} + base_unit_name;
    ret[ul::math::tera] = std::string{ul::math::tera_symb} + base_unit_name;
    ret[ul::math::peta] = std::string{ul::math::peta_symb} + base_unit_name;
    ret[ul::math::exa] = std::string{ul::math::exa_symb} + base_unit_name;

    return ret;
}

//! If you need just SI prefixes, you can use create_map_ratio_SIprefixunitname() for map_ratio_prefixunitname.
//! Throws Unit::err_no_string_provided_for_ratio if there is no string for the initial or target
//! ratio in the map. That would make the class useless.
class Unit {
public:
    struct ErrNoStringProvidedForRatio : public std::invalid_argument {
        explicit ErrNoStringProvidedForRatio(const ul::math::Rational& r)
            : std::invalid_argument("no string provided for ratio " + ul::to_string(r.as_floating_point<double>())) {
        }
    };

    /** Throws Unit::err_no_string_provided_for_ratio if there is no string for the initial
        ratio in the map. That would make the class useless. Also throws std::invalid_argument if a less or equal
        to zero ratio is contained within the map.
        Proper Rational's are expected. Also ratio needs to be > 0.
        Constructing the class with default parameters is only reasonable for testing purposes or temporary jump starts
        to construct other things.*/
    explicit Unit(
        const ul::math::Rational& ratio = ul::math::one, const MapRationalString& map_ratio_prefixunitname = {})
#if !UL_HAS_NO_CPP11_NOEXCEPT
/*noexcept(false)*/
#endif
        : ratio_(ratio)
        , ratio_prefixunitname_(
              map_ratio_prefixunitname.empty() ? simple_noop_default_ratio_map() : map_ratio_prefixunitname) {
        expect_valid_ratio(ratio);
        const auto invalid_one_it = std::find_if(
            std::begin(this->ratio_prefixunitname_), std::end(this->ratio_prefixunitname_),
            [](const std::pair<ul::math::Rational, std::string>& rs) {
                return rs.first <= ul::math::Rational{};
            });
        if (invalid_one_it != ratio_prefixunitname_.end())
            throw std::invalid_argument("ratio <= zero found in passed map");
    }

    ~Unit() {
        expect_valid_ratio(this->ratio_);
    }

    [[nodiscard]] std::string get_string() const {
        return this->ratio_prefixunitname_.at(this->ratio_);
    }

    [[nodiscard]] ul::math::Rational get_ratio() const {
        return this->ratio_;
    }

    /** Throws Unit::err_no_string_provided_for_ratio if there is no string for the target_ratio
        in the map. That would make the class useless.*/
    template <typename ValueType>
    //  ValueType expected to be arithmetic
    ValueType convert_to_different_ratio(ValueType src, const ul::math::Rational& target_ratio) const {
        static_assert(
            std::is_arithmetic<ValueType>::value, "only arithmetic (integral or floating point) types allowed");
        expect_valid_ratio(target_ratio);
        return static_cast<ValueType>(src * (this->ratio_ / target_ratio).as_floating_point<double>());
    }

    void switch_ratio(const ul::math::Rational& r) {
        expect_valid_ratio(r);
        this->ratio_ = r;
    }

    //! Ensures a return ratio valid to be set again via switch_ratio.
    template <typename ValueType>
    //  ValueType expected to be arithmetic
    ul::math::Rational find_optimized_ratio(ValueType val) const {
        static_assert(
            std::is_arithmetic<ValueType>::value, "only arithmetic (integral or floating point) types allowed");

        std::vector<ul::math::Rational> ratios;
        std::transform(
            std::begin(this->ratio_prefixunitname_), std::end(this->ratio_prefixunitname_), std::back_inserter(ratios),
            [](const std::pair<ul::math::Rational, std::string>& elem) {
                return elem.first;
            });

        std::sort(std::begin(ratios), std::end(ratios));

        const auto current_ratio_it = std::find(std::begin(ratios), std::end(ratios), this->ratio_);
        UL_ASSERT(current_ratio_it != std::end(ratios));

        const auto optim_ratio_it =
            std::find_if(std::begin(ratios), current_ratio_it, [val](const ul::math::Rational& r) {
                return val <= r.as_floating_point<double>();
            });

        if (optim_ratio_it != current_ratio_it) {
            expect_valid_ratio(*optim_ratio_it);
            return *optim_ratio_it;
        }

        const auto current_ratio_revit =
            std::reverse_iterator<std::vector<ul::math::Rational>::iterator>{current_ratio_it};
        const auto optim_ratio_revit =
            std::find_if(ratios.rbegin(), current_ratio_revit, [val](const ul::math::Rational& r) {
                return val >= r.as_floating_point<double>();
            });

        if (optim_ratio_revit != current_ratio_revit) {
            expect_valid_ratio(*optim_ratio_it);
            return *optim_ratio_revit;
        } else {
            expect_valid_ratio(*current_ratio_it);
            return *current_ratio_it;
        }
    }

private:
    ul::math::Rational ratio_;
    MapRationalString ratio_prefixunitname_;

    static const MapRationalString& simple_noop_default_ratio_map() {
        static const MapRationalString instance{create_map_ratio_simple()};
        return instance;
    }

    /** Return value only needed for calls within assertions. It doesn't need to indicate failure.
        The function itself does.*/
    void expect_valid_ratio(const ul::math::Rational& r) const {
        if (ratio_prefixunitname_.find(r) == ratio_prefixunitname_.end())
            throw ErrNoStringProvidedForRatio(r);
        UL_EXPECT(r > ul::math::Rational{});
    }
};

//! Describes a physical quantity with unit. Values have to be stored separately to avoid redundancy.
class Quantity {
public:
    Quantity(std::string quantity_name, const Unit& unit)
        : unit_{unit}
        , q_name_{std::move(quantity_name)} {
    }

    [[nodiscard]] std::string get_name() const {
        return this->q_name_;
    }

    Unit& get_unit() {
        return this->unit_;
    }

    [[nodiscard]] const Unit& get_unit() const {
        return const_cast<Quantity*>(this)->get_unit();
    }

private:
    Unit unit_;
    std::string q_name_;
};

//    requires WhatConcrete to be convertible to BaseType
// template <class WhatConcrete, typename BaseType = double>
// class QuantityValue
//{
// public:
//    Quantity(WhatConcrete& val);
//};

} // namespace mb::too::math


#endif
