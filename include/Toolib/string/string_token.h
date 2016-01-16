// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef STRING_TOKEN_H_INCL_uwireiwubefeubf
#define STRING_TOKEN_H_INCL_uwireiwubefeubf

#ifndef TOO_NO_DEPENDENCIES
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN" // <- doesn't work, why?!
#include <boost/algorithm/string.hpp>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#endif
#include <string>
#include <vector>

namespace too
{
namespace str
{
inline void tokenizeString(const std::string& s, const std::string& delimiters, std::vector<std::string>& out)
{
#ifndef TOO_NO_DEPENDENCIES
    boost::split(out, s, boost::is_any_of(delimiters));
#else
    size_t pos_start = s.find_first_not_of(delimiters);
    size_t pos_end = s.find_first_of(delimiters, pos_start);
    while (pos_start != std::string::npos)
    {
        out.push_back(s.substr(pos_start, pos_end - pos_start));
        pos_start = s.find_first_not_of(delimiters, pos_end);
        pos_end   = s.find_first_of(delimiters, pos_start);
    }
#endif
}
}
}

#endif
