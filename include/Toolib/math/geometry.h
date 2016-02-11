// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef GEOMETRY_H_dgiuntgh87xtg3487tg3tgfvscd
#define GEOMETRY_H_dgiuntgh87xtg3487tg3tgfvscd

#include <array>
#include <cmath>
#include <type_traits>
#include <initializer_list>
#include "Toolib/math/quantity_unit.h"
#include "Toolib/math/scale.h"
#include "Toolib/optional.h"


namespace too
{
namespace math
{

namespace
{
using DefaultValueType = double;
}

using Dimension = int;

template <typename T = DefaultValueType>
using Coord_         = too::enable_if_t<std::is_arithmetic<T>::value, T>;
using Coord          = Coord_<>;

template <typename T = DefaultValueType>
using RelativeCoord_ = too::enable_if_t<std::is_arithmetic<T>::value, T>;
using RelativeCoord  = RelativeCoord_<>;

template <typename T = DefaultValueType>
using Length_        = too::enable_if_t<std::is_arithmetic<T>::value, T>;
using Length         = Length_<>;

template <typename T = DefaultValueType>
using Width_         = Length_<T>;
using Width          = Width_<>;

template <typename T = DefaultValueType>
using Height_        = Length_<T>;
using Height         = Height_<>;

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
    std::array<T, dim> x{};

    Point_() : x{} {}

    Point_(std::initializer_list<T> init) { this->x = init; }

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
    T x{};
    T y{};

    Point_() : x{}, y{} {}

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
    Vector_<T, dim>& operator+=(const Vector_<T, dim>&)
    {
        // todo ...addition of rhs to *this...
        return *this;
    }
    Vector_<T, dim>& operator-=(const Vector_<T, dim>&)
    {
        // todo ...subtraction of rhs from *this...
        return *this;
    }
};

//! For convenience.
using Vector = Vector_<>;

//! Cf. LineSegment for convenience.
template <typename T = Coord, Dimension dim = 2>
struct LineSegment_
{
    Point_<T, dim> p1;
    Point_<T, dim> p2;
};

//! For convenience.
using LineSegment = LineSegment_<>;

//! Rectangle in 2D, in higher dimensions would be called hyperrectangle. Cf. Rectangle for convenience.
template <typename T = Coord, CoordOrigin origin = CoordOrigin::left_top>
struct Rectangle_
{
    Point_<T, 2> top_left;
    Point_<T, 2> bottom_right;

    Rectangle_(std::initializer_list<Point_<T, 2>> init)
    {
        auto it        = init.begin();
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

    Width_<T> width() const { return std::abs(bottom_right.x - top_left.x); }
    Height_<T> height() const { return std::abs(bottom_right.y - top_left.y); }
};

//! For convenience.
using Rectangle = Rectangle_<>;
}
}

#endif
