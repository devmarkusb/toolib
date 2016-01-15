// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef STRING_TRIM_H_INCL_cidusbfi243i7324
#define STRING_TRIM_H_INCL_cidusbfi243i7324

#include <string>
#ifndef TOO_NO_DEPENDENCIES
#include <boost/algorithm/string.hpp>
#else
//#include <locale>
//#include <algorithm>
//#include <functional>
#endif

namespace too
{
namespace str
{
inline std::string& ltrim(std::string& s, const std::string& trimchars = " \t\n")
{
    // Following suggestion from From Evan Teran, stackoverflow doesn't compile.
    // s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
    s.erase(0, s.find_first_not_of(trimchars));
    return s;
}
inline std::string& rtrim(std::string& s, const std::string& trimchars = " \t\n")
{
    // s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
    s.erase(s.find_last_not_of(trimchars) + 1);
    return s;
}
inline std::string& trim(std::string& s)
{
#ifndef TOO_NO_DEPENDENCIES
    boost::trim(s);
#else
    ltrim(rtrim(s));
#endif
    return s;
}
}
} // too

#endif
