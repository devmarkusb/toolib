// Markus Borris, 2015
// This file is part of my Toolib library.

//!
/**
*/
//! \file

#ifndef OPERATOR_OVERLOADING_H_INCL_dsjfgnxe8iunhgx873xgnf387gfn4
#define OPERATOR_OVERLOADING_H_INCL_dsjfgnxe8iunhgx873xgnf387gfn4

#error Do not include this header file! The idea is to use it just as a source for copy&paste.

#include <iostream>

namespace too
{

struct T
{
    T() = default;
    ~T() noexcept = default;
    T(const T&) = default;
    T(T&& other) noexcept = default;
    T& operator=(const T& other)
    {
        T tmp(other);
        *this = std::move(tmp);
        return *this;
    }
    T& operator=(T&&) noexcept = default;


    T& operator++()
    {
        // todo ...
        return *this;
    }
    T operator++(int)
    {
        T tmp(*this);
        operator++();
        return tmp;
    }
    T& operator--()
    {
        // todo ...
        return *this;
    }
    T operator--(int)
    {
        T tmp(*this);
        operator--();
        return tmp;
    }


    T& operator+=(const T&)
    {
        // todo ...addition of rhs to *this...
        return *this;
    }
    T& operator-=(const T&)
    {
        // todo ...subtraction of rhs from *this...
        return *this;
    }
    T& operator*=(const T&)
    {
        // todo ...multiplication of rhs to *this...
        return *this;
    }
    T& operator/=(const T&)
    {
        // todo ...division of *this by rhs...
        return *this;
    }
    // unary minus
    T operator-()
    {
        T tmp{};
        // todo ...assign -members of *this to tmp's members...
        return tmp;
    }
};

inline T operator+(T lhs, const T& rhs)
{
    lhs += rhs;
    return lhs;
}
inline T operator-(T lhs, const T& rhs)
{
    lhs -= rhs;
    return lhs;
}
inline T operator*(T lhs, const T& rhs)
{
    lhs *= rhs;
    return lhs;
}
inline T operator/(T lhs, const T& rhs)
{
    lhs /= rhs;
    return lhs;
}


inline bool operator==(const T&, const T&)
{
    return true /*todo ...comparison...*/;
}
inline bool operator!=(const T& lhs, const T& rhs)
{
    return !operator==(lhs, rhs);
}
inline bool operator<(const T&, const T&)
{
    return false /*todo ...comparison...*/;
}
inline bool operator>(const T& lhs, const T& rhs)
{
    return operator<(rhs, lhs);
}
inline bool operator<=(const T& lhs, const T& rhs)
{
    return !operator>(lhs, rhs);
}
inline bool operator>=(const T& lhs, const T& rhs)
{
    return !operator<(lhs, rhs);
}


inline std::ostream& operator<<(std::ostream& os, const T&)
{
    // todo ...write obj to stream...
    return os;
}

inline std::istream& operator>>(std::istream& is, T&)
{
    // todo ...read obj from stream...
    if (false /*todo no valid object of T found in stream*/)
        is.setstate(std::ios::failbit);
    return is;
}


//#########################################################################################

// For clearity, a separate struct for other kinds of overloads.
struct U
{
    // With C++11 'implicit explicit casts' are possible for bool, so we no longer need the
    // 'safe bool idiom'. But this doesn't work for enum (class)!?
    explicit operator bool() const
    {
        return true; // todo
    }
    // this is actually no longer needed in the bool context
    const bool operator!() const
    {
        return false; // todo
    }
};
}

#endif
