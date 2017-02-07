// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef FILE_H_sduifhg3gfy324n178fsffe4f
#define FILE_H_sduifhg3gfy324n178fsffe4f

#include "Toolib/assert.h"
#include <fstream>
#include <string>


namespace too
{
namespace file
{
//! \param retErrDetail returns error detail string or is empty on success.
//! Function \returns false on success.
template <class FStream>
bool fstream_failed(std::string& retErrDetail, const FStream& fs)
{
    retErrDetail.clear();
    if (fs)
        return false;
    if (fs.eof())
        retErrDetail = "eof";
    else if (fs.bad())
        retErrDetail = "bad";
    else if (fs.fail())
        retErrDetail = "fail";
    return true;
}

enum class operation
{
    save,
    load,
};
inline void throwError(const std::string& filePathNameExt, operation op, const std::string& retErrDetail)
{
    std::string s{filePathNameExt};
    s += " could not be ";
    switch (op)
    {
    case operation::save:
        s += "saved";
        break;
    case operation::load:
        s += "loaded";
        break;
    default:
        TOO_ASSERT(false); // op not supported
    }
    s += ", details: ";
    s += retErrDetail;
    throw std::runtime_error{s};
}
} // file
} // too

#endif
