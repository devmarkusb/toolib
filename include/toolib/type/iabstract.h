//! \file Keep it C-ABI linkable.

#ifndef IABSTRACT_H_AJKSHUIHNR82374R72FF23RNY8FDNBWYE
#define IABSTRACT_H_AJKSHUIHNR82374R72FF23RNY8FDNBWYE

#include "../config.h"

namespace mb::too {
using TInBuffer = void*;
using TOutBuffer = void*;

struct Iabstract {
    virtual ~Iabstract() = default;

    virtual bool get_parameter(const char* name, TOutBuffer value) const = 0;
    virtual bool set_parameter(const char* name, TInBuffer value) = 0;
    virtual bool execute(const char* command, const TInBuffer params[] = nullptr, TOutBuffer retvalue = nullptr) = 0;
};
} // namespace mb::too

TOO_HEADER_END

#endif
