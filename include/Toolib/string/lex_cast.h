// Markus Borris, 2011-2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef LEX_CAST_H_INCL_sopaiem3p2oium32omo
#define LEX_CAST_H_INCL_sopaiem3p2oium32omo

#ifndef TOO_NO_DEPENDENCIES
#include <boost/lexical_cast.hpp>
#else
#include <sstream>
#include <exception>
#endif
#include "Toolib/PPDEFS.h"
#include "Toolib/std/std_extensions.h"


namespace too
{
class ErrBadLexCast : public std::bad_cast
{
};

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
