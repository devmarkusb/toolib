// 2016

//!
/**
 */
//! \file

#ifndef GEOMETRY_H_dgiuntgh87xtg3487tg3tgfvscd
#define GEOMETRY_H_dgiuntgh87xtg3487tg3tgfvscd

#include "toolib/math/quantity_unit.h"
#include "toolib/math/scale.h"
#include "ul/ul.h"
#include <array>
#include <cmath>
#include <initializer_list>
#include <type_traits>


namespace mb::too::math
{
namespace detail
{
using DefaultValueType = double;
} // namespace detail

using Dimension = int;

template <typename T = detail::DefaultValueType>
using Coord_ = ul::enable_if_t<std::is_arithmetic<T>::value, T>;
using Coord = Coord_<>;

template <typename T = detail::DefaultValueType>
using RelativeCoord_ = ul::enable_if_t<std::is_arithmetic<T>::value, T>;
using RelativeCoord = RelativeCoord_<>;

template <typename T = detail::DefaultValueType>
using Length_ = ul::enable_if_t<std::is_arithmetic<T>::value, T>;
using Length = Length_<>;

template <typename T = detail::DefaultValueType>
using Width_ = Length_<T>;
using Width = Width_<>;

template <typename T = detail::DefaultValueType>
using Height_ = Length_<T>;
using Height = Height_<>;

//! For easier understanding the choice of the coordinate system axes directions
//! is described by visualizing screen corners (so everyone knows, that left_top
//! means x running from left to right, and y from top to bottom).
enum class CoordOrigin
{
    left_top,
    left_bottom,
};

//! Initialized to T(), ..., T(). Cf. Point.
template <typename T = Coord, Dimension dim = 2>
struct Point_
{
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");
    static_assert(dim >= 1, "expects d >= 1");

    std::array<T, dim> x{};

    Point_()
        : x{}
    {
    }

    Point_(std::initializer_list<T> init)
    {
        this->x = init;
    }

    Point_& operator=(std::initializer_list<T> init)
    {
        Point_<T, dim> tmp{init};
        *this = std::move(tmp);
        return *this;
    }
};

//! For convenience.
using Point = Point_<>;

//! More convenience for two dimensions.
template <typename T>
struct Point_<T, 2>
{
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");

    T x{};
    T y{};

    Point_()
        : x{}
        , y{}
    {
    }

    Point_(std::initializer_list<T> init)
    {
        auto it = init.begin();
        this->x = *it;
        ++it;
        this->y = *it;
    }

    Point_& operator=(std::initializer_list<T> init)
    {
        Point_<T, 2> tmp{init};
        *this = std::move(tmp);
        return *this;
    }
};

//! Cf. Point_.
template <typename T = Coord, Dimension dim = 2>
struct Vector_ : public Point_<T, dim>
{
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");
    static_assert(dim >= 1, "expects d >= 1");

    Vector_<T, dim>& operator+=(const Vector_<T, dim>&)
    {
        throw ul::not_implemented{"Vector_ +="};
        // return *this;
    }
    Vector_<T, dim>& operator-=(const Vector_<T, dim>&)
    {
        throw ul::not_implemented{"Vector_ -="};
        // return *this;
    }
};

//! For convenience.
using Vector = Vector_<>;

//! Cf. LineSegment for convenience.
template <typename T = Coord, Dimension dim = 2>
struct LineSegment_
{
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");
    static_assert(dim >= 1, "expects d >= 1");

    Point_<T, dim> p1;
    Point_<T, dim> p2;
};

//! For convenience.
using LineSegment = LineSegment_<>;

//! Rectangle in 2D, in higher dimensions would be called hyperrectangle. Cf. Rectangle for convenience.
template <typename T = Coord, CoordOrigin origin = CoordOrigin::left_top>
struct Rectangle_
{
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");

    Point_<T, 2> top_left;
    Point_<T, 2> bottom_right;

    Rectangle_(std::initializer_list<Point_<T, 2>> init)
    {
        auto it = init.begin();
        this->top_left = *it;
        ++it;
        this->bottom_right = *it;
    }

    Rectangle_& operator=(std::initializer_list<Point_<T, 2>> init)
    {
        Rectangle_<T, origin> tmp{init};
        *this = std::move(tmp);
        return *this;
    }

    static const CoordOrigin coord_origin = origin;

    Width_<T> width() const
    {
        return std::abs(bottom_right.x - top_left.x);
    }
    Height_<T> height() const
    {
        return std::abs(bottom_right.y - top_left.y);
    }
};

//! For convenience.
using Rectangle = Rectangle_<>;


//!
template <typename T = Width>
struct Margins_
{
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");

    T left{};
    T right{};
    T top{};
    T bottom{};

    Margins_() = default;
    Margins_(T left, T right, T top, T bottom)
        : left{left}
        , right{right}
        , top{top}
        , bottom{bottom}
    {
    }
};

//! For convenience.
using Margins = Margins_<>;
} // namespace mb::too::math

#endif
