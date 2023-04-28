//! \file

#ifndef PERCENT_H_kjdghnxzre8hgnf83xgfn34gx
#define PERCENT_H_kjdghnxzre8hgnf83xgfn34gx

#include "../config.h"

namespace mb::too::math {
using Percent = double;
using Factor = double;
const Percent one_hundred_percent = 100.0;
const Factor factor_one = 1.0;
const Percent zero_percent = Percent{};
const Factor factor_zero = Factor{};
} // namespace mb::too::math

TOO_HEADER_END

#endif
