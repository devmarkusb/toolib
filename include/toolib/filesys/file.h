// Markus Borris, 2016
// This file is part of toolib library.

//!
/**
*/
//! \file

#ifndef FILE_H_sduifhg3gfy324n178fsffe4f
#define FILE_H_sduifhg3gfy324n178fsffe4f

#include "toolib/assert.h"
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>
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
    const auto errno_ = errno;
    std::stringstream ss;
    ss << filePathNameExt;
    ss << " could not be ";
    switch (op)
    {
    case operation::save:
        ss << "saved";
        break;
    case operation::load:
        ss << "loaded";
        break;
    default:
        TOO_ASSERT(false); // op not supported
    }
    ss << ", details: " << retErrDetail;
    ss << ", ec: " << errno_ << ", " << std::strerror(errno_);
    throw std::runtime_error{ss.str()};
}
} // file
} // too

#endif
