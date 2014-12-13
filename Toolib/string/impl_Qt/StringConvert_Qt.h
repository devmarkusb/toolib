// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3
#define STRINGCONVERT_QT_H_isadfnyiwungsfuzwigqf3

#include <QString>
#include "../../types.h"


namespace too
{
namespace implQt
{

//! QString to too::string
inline too::string qs2toos(QString s)
{
    too::static_assert_string_BitsPerChar_and_Encoding<too::string, 8>();
    return too::string(s.toUtf8().constData());
}

//! too::string to QString
inline QString toos2qs(too::string s)
{
    too::static_assert_string_BitsPerChar_and_Encoding<too::string, 8>();
    return QString::fromUtf8(s.c_str());
}

}
}

#endif // STRINGCONVERT_QT_H
