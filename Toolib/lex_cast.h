// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef LEX_CAST_H_INCL_sopaiem3p2oium32omo
#define LEX_CAST_H_INCL_sopaiem3p2oium32omo

#if TOO_USE_BOOST
#include <boost/lexical_cast.hpp>
#else
#include <sstream>
#include <exception>
#endif

namespace too
{
	class ErrBadLexCast : public std::bad_cast {};

	template<typename T, typename S> inline T lex_cast(const S& s)
	{
#if TOO_USE_BOOST
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
} // too

#endif