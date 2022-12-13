// 2016-17

//! \file


#ifndef QUANTITY_UNIT_H_dfhgxungh38hgxt38gb
#define QUANTITY_UNIT_H_dfhgxungh38hgxt38gb

#include "ratio.h"
#include "ul/ul.h"
#include <exception>
#include <map>
#include <ratio>
#include <string>
#include <type_traits>
#include <vector>


namespace mb::too::math
{
using Map_Rational_String = std::map<too::math::Rational, std::string>;

//! To create a simple default for constructing a Unit in cases you don't want to think about details.
inline Map_Rational_String create_map_ratio_simple(const std::string& base_unit_name = {})
{
    std::map<too::math::Rational, std::string> ret;
    ret[too::math::one] = too::math::one_symb + base_unit_name;
    return ret;
}

//! This is for all quantities that work with standard SI unit prefixes.
//! Unfortunately you will have to build custom maps for some quantity types, e.g. time, money,
//! even for masses you would like to write t instead of Mg; also internationalization plays a role
//! so that it is better to keep strings under your control.
/** \param only_factors_of_thousand resticts output to the most popular steps of thousands, factors of 10^3.
    That misses e.g. cm, dm.*/
inline Map_Rational_String create_map_ratio_SIprefixunitname(
    const std::string& base_unit_name, bool only_factors_of_thousand = true)
{
    Map_Rational_String ret;

    ret[too::math::atto] = too::math::atto_symb + base_unit_name;
    ret[too::math::femto] = too::math::femto_symb + base_unit_name;
    ret[too::math::pico] = too::math::pico_symb + base_unit_name;
    ret[too::math::nano] = too::math::nano_symb + base_unit_name;
    ret[too::math::micro] = too::math::micro_symb + base_unit_name;
    ret[too::math::milli] = too::math::milli_symb + base_unit_name;
    if (!only_factors_of_thousand)
    {
        ret[too::math::centi] = too::math::centi_symb + base_unit_name;
        ret[too::math::deci] = too::math::deci_symb + base_unit_name;
    }

    ret[too::math::one] = too::math::one_symb + base_unit_name;

    if (!only_factors_of_thousand)
    {
        ret[too::math::deca] = too::math::deka_symb + base_unit_name;
        ret[too::math::hecto] = too::math::hecto_symb + base_unit_name;
    }
    ret[too::math::kilo] = too::math::kilo_symb + base_unit_name;
    ret[too::math::mega] = too::math::mega_symb + base_unit_name;
    ret[too::math::giga] = too::math::giga_symb + base_unit_name;
    ret[too::math::tera] = too::math::tera_symb + base_unit_name;
    ret[too::math::peta] = too::math::peta_symb + base_unit_name;
    ret[too::math::exa] = too::math::exa_symb + base_unit_name;

    return ret;
}


//! If you need just SI prefixes, you can use create_map_ratio_SIprefixunitname() for map_ratio_prefixunitname.
//! Throws Unit::err_no_string_provided_for_ratio if there is no string for the initial or target
//! ratio in the map. That would make the class useless.
class Unit
{
public:
    struct err_no_string_provided_for_ratio : public std::invalid_argument
    {
        err_no_string_provided_for_ratio(const Rational& r)
            : std::invalid_argument("no string provided for ratio " + ul::to_string(r.asFloatingPoint<double>()))
        {
        }
    };

    /** Throws Unit::err_no_string_provided_for_ratio if there is no string for the initial
        ratio in the map. That would make the class useless. Also throws std::invalid_argument if a less or equal
        to zero ratio is contained within the map.
        Proper Rational's are expected. Also ratio needs to be > 0.
        Constructing the class with default parameters is only reasonable for testing purposes or temporary jump starts
        to construct other things.*/
    explicit Unit(
        const too::math::Rational& ratio = too::math::one, const Map_Rational_String& map_ratio_prefixunitname = {})
#if !UL_HAS_NO_CPP11_NOEXCEPT
/*noexcept(false)*/
#endif
        : ratio(ratio)
        , ratio_prefixunitname(
              map_ratio_prefixunitname.empty() ? simple_noop_default_ratio_map() : map_ratio_prefixunitname)
    {
        expectValidRatio(ratio);
        const auto invalid_one_it = std::find_if(
            std::begin(this->ratio_prefixunitname), std::end(this->ratio_prefixunitname),
            [](const std::pair<too::math::Rational, std::string>& rs) {
                return rs.first <= Rational{};
            });
        if (invalid_one_it != ratio_prefixunitname.end())
            throw std::invalid_argument("ratio <= zero found in passed map");
    }

    ~Unit()
    {
        expectValidRatio(this->ratio);
    }

    [[nodiscard]] std::string getString() const
    {
        return this->ratio_prefixunitname.at(this->ratio);
    }

    [[nodiscard]] Rational getRatio() const
    {
        return this->ratio;
    }

    /** Throws Unit::err_no_string_provided_for_ratio if there is no string for the target_ratio
        in the map. That would make the class useless.*/
    template <typename ValueType>
    //  ValueType expected to be arithmetic
    ValueType convertToDifferentRatio(ValueType src, const too::math::Rational& target_ratio) const
    {
        static_assert(
            std::is_arithmetic<ValueType>::value, "only arithmetic (integral or floating point) types allowed");
        expectValidRatio(target_ratio);
        return static_cast<ValueType>(src * (this->ratio / target_ratio).asFloatingPoint<double>());
    }

    void switchRatio(const too::math::Rational& r)
    {
        expectValidRatio(r);
        this->ratio = r;
    }

    //! Ensures a return ratio valid to be set again via switchRatio.
    template <typename ValueType>
    //  ValueType expected to be arithmetic
    Rational findOptimizedRatio(ValueType val) const
    {
        static_assert(
            std::is_arithmetic<ValueType>::value, "only arithmetic (integral or floating point) types allowed");

        std::vector<Rational> ratios;
        std::transform(
            std::begin(this->ratio_prefixunitname), std::end(this->ratio_prefixunitname), std::back_inserter(ratios),
            [](const std::pair<Rational, std::string>& elem) {
                return elem.first;
            });

        std::sort(std::begin(ratios), std::end(ratios));

        const auto current_ratio_it = std::find(std::begin(ratios), std::end(ratios), this->ratio);
        UL_ASSERT(current_ratio_it != std::end(ratios));

        const auto optim_ratio_it = std::find_if(std::begin(ratios), current_ratio_it, [val](const Rational& r) {
            return val <= r.asFloatingPoint<double>();
        });

        if (optim_ratio_it != current_ratio_it)
        {
            expectValidRatio(*optim_ratio_it);
            return *optim_ratio_it;
        }

        const auto current_ratio_revit = std::reverse_iterator<std::vector<Rational>::iterator>{current_ratio_it};
        const auto optim_ratio_revit = std::find_if(ratios.rbegin(), current_ratio_revit, [val](const Rational& r) {
            return val >= r.asFloatingPoint<double>();
        });

        if (optim_ratio_revit != current_ratio_revit)
        {
            expectValidRatio(*optim_ratio_it);
            return *optim_ratio_revit;
        }
        else
        {
            expectValidRatio(*current_ratio_it);
            return *current_ratio_it;
        }
    }

private:
    too::math::Rational ratio;
    Map_Rational_String ratio_prefixunitname;

    static const Map_Rational_String& simple_noop_default_ratio_map()
    {
        static const Map_Rational_String instance{create_map_ratio_simple()};
        return instance;
    }

    /** \return value only needed for calls within assertions. It doesn't need to indicate failure.
        The function itself does.*/
    void expectValidRatio(const too::math::Rational& r) const
    {
        if (ratio_prefixunitname.find(r) == ratio_prefixunitname.end())
            throw err_no_string_provided_for_ratio(r);
        UL_EXPECT(r > Rational{});
    }
};


//! Describes a physical quantity with unit. Values have to be stored separately to avoid redundancy.
class Quantity
{
public:
    Quantity(std::string quantity_name, const Unit& unit)
        : unit{unit}
        , q_name{std::move(quantity_name)}
    {
    }

    [[nodiscard]] std::string getName() const
    {
        return this->q_name;
    }
    Unit& getUnit()
    {
        return this->unit;
    }
    [[nodiscard]] const Unit& getUnit() const
    {
        return const_cast<Quantity*>(this)->getUnit();
    }

private:
    Unit unit;
    std::string q_name;
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
