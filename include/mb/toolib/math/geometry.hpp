//! \file

#ifndef GEOMETRY_HPP_DGIUNTGH87XTG3487TG3TGFVSCD
#define GEOMETRY_HPP_DGIUNTGH87XTG3487TG3TGFVSCD

#include "../config.hpp"
#include "mb/toolib/math/quantity_unit.hpp"
#include "mb/toolib/math/scale.hpp"
#include "mb/ul/ul.hpp"
#include <array>
#include <cstddef>
#include <cmath>
#include <initializer_list>
#include <type_traits>

namespace mb::too::math {
namespace detail {
using DefaultValueType = double;
} // namespace detail

using Dimension = int;

template <typename T = detail::DefaultValueType>
using CoordImpl = ul::enable_if_t<std::is_arithmetic<T>::value, T>;
using Coord = CoordImpl<>;

template <typename T = detail::DefaultValueType>
using RelativeCoordImpl = ul::enable_if_t<std::is_arithmetic<T>::value, T>;
using RelativeCoord = RelativeCoordImpl<>;

template <typename T = detail::DefaultValueType>
using LengthImpl = ul::enable_if_t<std::is_arithmetic<T>::value, T>;
using Length = LengthImpl<>;

template <typename T = detail::DefaultValueType>
using WidthImpl = LengthImpl<T>;
using Width = WidthImpl<>;

template <typename T = detail::DefaultValueType>
using HeightImpl = LengthImpl<T>;
using Height = HeightImpl<>;

//! For easier understanding the choice of the coordinate system axes directions
//! is described by visualizing screen corners (so everyone knows, that left_top
//! means x running from left to right, and y from top to bottom).
enum class CoordOrigin {
    left_top,
    left_bottom,
};

//! Initialized to T(), ..., T(). Cf. Point.
template <typename T = Coord, Dimension dim = 2>
struct PointImpl {
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");
    static_assert(dim >= 1, "expects d >= 1");

    std::array<T, static_cast<std::size_t>(dim)> x{};

    PointImpl()
        : x{} {
    }

    PointImpl(std::initializer_list<T> init) {
        this->x = init;
    }

    PointImpl& operator=(std::initializer_list<T> init) {
        PointImpl<T, dim> tmp{init};
        *this = std::move(tmp);
        return *this;
    }
};

//! For convenience.
using Point = PointImpl<>;

//! More convenience for two dimensions.
template <typename T>
struct PointImpl<T, 2> {
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");

    T x{};
    T y{};

    PointImpl()
        : x{}
        , y{} {
    }

    PointImpl(std::initializer_list<T> init) {
        auto it = init.begin();
        this->x = *it;
        ++it;
        this->y = *it;
    }

    PointImpl& operator=(std::initializer_list<T> init) {
        PointImpl<T, 2> tmp{init};
        *this = std::move(tmp);
        return *this;
    }
};

//! Cf. Point_.
template <typename T = Coord, Dimension dim = 2>
struct VectorImpl : public PointImpl<T, dim> {
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");
    static_assert(dim >= 1, "expects d >= 1");

    VectorImpl<T, dim>& operator+=(const VectorImpl<T, dim>&) {
        throw ul::NotImplemented{"Vector_ +="};
        // return *this;
    }

    VectorImpl<T, dim>& operator-=(const VectorImpl<T, dim>&) {
        throw ul::NotImplemented{"Vector_ -="};
        // return *this;
    }
};

//! For convenience.
using Vector = VectorImpl<>;

//! Cf. LineSegment for convenience.
template <typename T = Coord, Dimension dim = 2>
struct LineSegmentImpl {
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");
    static_assert(dim >= 1, "expects d >= 1");

    PointImpl<T, dim> p1;
    PointImpl<T, dim> p2;
};

//! For convenience.
using LineSegment = LineSegmentImpl<>;

//! Rectangle in 2D, in higher dimensions would be called hyperrectangle. Cf. Rectangle for convenience.
template <typename T = Coord, CoordOrigin origin = CoordOrigin::left_top>
struct RectangleImpl {
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");

    PointImpl<T, 2> top_left;
    PointImpl<T, 2> bottom_right;

    RectangleImpl(std::initializer_list<PointImpl<T, 2>> init) {
        auto it = init.begin();
        this->top_left = *it;
        ++it;
        this->bottom_right = *it;
    }

    RectangleImpl& operator=(std::initializer_list<PointImpl<T, 2>> init) {
        RectangleImpl<T, origin> tmp{init};
        *this = std::move(tmp);
        return *this;
    }

    static const CoordOrigin coord_origin = origin;

    WidthImpl<T> width() const {
        return std::abs(bottom_right.x - top_left.x);
    }

    HeightImpl<T> height() const {
        return std::abs(bottom_right.y - top_left.y);
    }
};

//! For convenience.
using Rectangle = RectangleImpl<>;

//!
template <typename T = Width>
struct MarginsImpl {
    static_assert(std::is_arithmetic<T>::value, "T has to be an arithmetic type");

    T left{};
    T right{};
    T top{};
    T bottom{};

    MarginsImpl() = default;

    MarginsImpl(T left, T right, T top, T bottom)
        : left{left}
        , right{right}
        , top{top}
        , bottom{bottom} {
    }
};

//! For convenience.
using Margins = MarginsImpl<>;
} // namespace mb::too::math


#endif
