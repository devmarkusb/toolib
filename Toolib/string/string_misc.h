// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef STRING_MISC_H_INCL_ciubhwduibgwi37
#define STRING_MISC_H_INCL_ciubhwduibgwi37

#include <string>
#include <ctype>
#include <algorithm>

namespace too
{
	namespace str
	{
		inline std::string& makeUpper(std::string &s)
		{
			std::transform(s.begin(), s.end(), s.begin(), toupper);
		}

		inline std::string& makeLower(std::string &s)
		{
			std::transform(s.begin(), s.end(), s.begin(), tolower);
		}
	}
} // too

#endif