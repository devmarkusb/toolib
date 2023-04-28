//! \file Keep it C-ABI linkable.

#ifndef IABSTRACT_H_ajkshuihnr82374r72ff23rny8fdnbwye
#define IABSTRACT_H_ajkshuihnr82374r72ff23rny8fdnbwye

#include "../config.h"

namespace mb::too {
using TInBuffer = void*;
using TOutBuffer = void*;

struct iabstract {
    virtual ~iabstract() = default;

    virtual bool getParameter(const char* name, TOutBuffer value) const = 0;
    virtual bool setParameter(const char* name, TInBuffer value) = 0;
    virtual bool execute(const char* command, const TInBuffer params[] = nullptr, TOutBuffer retvalue = nullptr) = 0;
};
} // namespace mb::too

TOO_HEADER_END

#endif
