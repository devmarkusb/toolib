// Markus Borris, 2015
// This file is part of Toolib library. Open source.

//!
/** Keep it C-ABI linkable.
*/
//! \file

#ifndef IABSTRACT_H_INCL_ajkshuihnr82374r72ff23rny8fdnbwye
#define IABSTRACT_H_INCL_ajkshuihnr82374r72ff23rny8fdnbwye

namespace too
{

using TInBuffer  = void*;
using TOutBuffer = void*;

struct IAbstract
{
    virtual ~IAbstract() = default;

    virtual bool GetParameter(const char* name, TOutBuffer value) const = 0;
    virtual bool SetParameter(const char* name, const TInBuffer value) = 0;
    virtual bool Execute(const char* command, const TInBuffer params[] = nullptr, TOutBuffer retvalue = nullptr) = 0;
};
}

#endif
