// Markus Borris, 2011-2018
// This file is part of toolib library.

//!
/**
*/
//! \file

#ifndef LEX_CAST_H_INCL_sopaiem3p2oium32omo
#define LEX_CAST_H_INCL_sopaiem3p2oium32omo

#include "toolib/PPDEFS.h"
#include "toolib/std/std_extensions.h"
#include <exception>
#include <sstream>


namespace too
{
class ErrBadLexCast : public std::bad_cast
{
};

//! Converts 'arbitrary' types from or to std::string.
//! There is also the non-throwing version lex_cast.
/** It is even possible to cast from float or int or such. Although one might
    need to be sceptic about the use-case of trying such conversions the
    lexicographical way. They also would need to be fail-safe input-wise,
    that is 42.1f doesn't successfully convert to 42 or whatever.*/
template <typename T, typename S>
inline T lex_cast_Throw(const S& s)
{
    std::stringstream strs;
    strs << s;
    T t;
    strs >> t;
    if (strs)
        return t;
    else
        throw ErrBadLexCast();
}

template <typename T, typename S>
inline T lex_cast(const S& s) noexcept
{
    try
    {
        return lex_cast_Throw<T, S>(s);
    }
    catch (const ErrBadLexCast&)
    {
        TOO_NOOP;
    }
    return T{};
}
} // too

#endif
