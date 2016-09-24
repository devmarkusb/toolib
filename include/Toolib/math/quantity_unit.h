// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef QUANTITY_UNIT_H_dfhgxungh38hgxt38gb
#define QUANTITY_UNIT_H_dfhgxungh38hgxt38gb

#include <string>
#include <map>
#include <ratio>
#include <type_traits>
#include <exception>
#include "Toolib/math/ratio.h"
#include "Toolib/std/std_extensions.h"


namespace too
{
namespace math
{

//! This is for all quantities that work with standard SI unit prefixes.
//! Unfortunately you will have to build custom maps for some quantity types, e.g. time, money,
//! even for masses you would like to write t instead of Mg; also internationalization plays a role
//! so that it is better to keep strings under your control.
inline std::map<too::math::Rational, std::string> create_map_ratio_SIprefixunitname(const std::string& base_unit_name)
{
    std::map<too::math::Rational, std::string> ret;

    ret[too::math::atto]  = too::math::atto_symb + base_unit_name;
    ret[too::math::femto] = too::math::femto_symb + base_unit_name;
    ret[too::math::pico]  = too::math::pico_symb + base_unit_name;
    ret[too::math::nano]  = too::math::nano_symb + base_unit_name;
    ret[too::math::micro] = too::math::micro_symb + base_unit_name;
    ret[too::math::milli] = too::math::milli_symb + base_unit_name;
    ret[too::math::centi] = too::math::centi_symb + base_unit_name;
    ret[too::math::deci]  = too::math::deci_symb + base_unit_name;

    ret[too::math::one] = too::math::one_symb + base_unit_name;

    ret[too::math::deka]  = too::math::deka_symb + base_unit_name;
    ret[too::math::hecto] = too::math::hecto_symb + base_unit_name;
    ret[too::math::kilo]  = too::math::kilo_symb + base_unit_name;
    ret[too::math::mega]  = too::math::mega_symb + base_unit_name;
    ret[too::math::giga]  = too::math::giga_symb + base_unit_name;
    ret[too::math::tera]  = too::math::tera_symb + base_unit_name;
    ret[too::math::peta]  = too::math::peta_symb + base_unit_name;
    ret[too::math::exa]   = too::math::exa_symb + base_unit_name;

    return ret;
}


//! Passed object to \param map_ratio_prefixunitname needs to be alive for the whole life time of Unit.
//! To avoid heavy copying we just refer to that. If you need just SI prefixes, you can use
//! create_map_ratio_SIprefixunitname().
//! Throws Unit::err_no_string_provided_for_ratio if there is no string for the initial or target
//! ratio in the map. That would make the class useless.
class Unit
{
public:
    struct err_no_string_provided_for_ratio : public std::exception
    {
    };

    //! Throws Unit::err_no_string_provided_for_ratio if there is no string for the initial
    //! \param ratio in the map. That would make the class useless.
    Unit(const too::math::Rational& ratio, const std::map<too::math::Rational, std::string>& map_ratio_prefixunitname)
#if !TOO_HAS_NO_CPP11_NOEXCEPT
        noexcept(false)
#endif
        : ratio(ratio), ratio_prefixunitname(map_ratio_prefixunitname)
    {
        if (map_ratio_prefixunitname.find(ratio) == map_ratio_prefixunitname.end())
            throw err_no_string_provided_for_ratio();
    }

    ~Unit() = default;
    Unit(const Unit& other) : ratio(other.ratio), ratio_prefixunitname{other.ratio_prefixunitname} {}
    Unit& operator=(const Unit& other)
    {
        Unit tmp{other};
        *this = std::move(tmp);
        return *this;
    }
    Unit(Unit&& other) : ratio(std::move(other.ratio)), ratio_prefixunitname{other.ratio_prefixunitname} {}
    Unit& operator=(Unit&& other)
    {
        this->ratio                 = std::move(other.ratio);
        Map_Rational_String& helper = const_cast<Map_Rational_String&>(this->ratio_prefixunitname);
        helper                      = other.ratio_prefixunitname;
        return *this;
    }

    std::string getString() const { return this->ratio_prefixunitname.at(this->ratio); }

    //! Throws Unit::err_no_string_provided_for_ratio if there is no string for the \param target_ratio
    //! in the map. That would make the class useless.
    template <typename ValueType>
    ValueType convertTo(ValueType src, const too::math::Rational& target_ratio)
#if !TOO_HAS_NO_CPP11_NOEXCEPT
        noexcept(false)
#endif
    {
        static_assert(
            std::is_arithmetic<ValueType>::value, "only arithmetic (integral or floating point) types allowed");
        TOO_EXPECT(target_ratio.num);
        if (this->ratio_prefixunitname.find(target_ratio) == this->ratio_prefixunitname.end())
            throw err_no_string_provided_for_ratio();
        ValueType ret = src * (this->ratio / target_ratio);
        this->ratio   = target_ratio;
        return ret;
    }

private:
    too::math::Rational ratio;
    using Map_Rational_String = std::map<too::math::Rational, std::string>;
    const Map_Rational_String& ratio_prefixunitname;
};


//! Describes a physical quantity with unit. Values have to be stored separately to avoid redundancy.
class Quantity
{
public:
    Quantity(const std::string& quantity_name, const Unit& unit) : unit(unit), q_name(quantity_name) {}

    std::string getName() const { return this->q_name; }
    Unit& getUnit() { return this->unit; }

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
}
}

#endif
