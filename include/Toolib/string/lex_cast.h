// Markus Borris, 2011-2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef LEX_CAST_H_INCL_sopaiem3p2oium32omo
#define LEX_CAST_H_INCL_sopaiem3p2oium32omo

#include "Toolib/PPDEFS.h"
#include "Toolib/std/std_extensions.h"
#ifndef TOO_NO_DEPENDENCIES
#include <boost/lexical_cast.hpp>
#else
#include <exception>
#include <sstream>
#endif


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
#ifndef TOO_NO_DEPENDENCIES
    try
    {
        T t;
        t = boost::lexical_cast<T>(s);
        return t;
    }
    catch (boost::bad_lexical_cast)
    {
        throw ErrBadLexCast();
    }
#else
    std::stringstream strs;
    strs << s;
    T t;
    strs >> t;
    if (strs)
        return t;
    else
        throw ErrBadLexCast();
#endif
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
}

#endif
