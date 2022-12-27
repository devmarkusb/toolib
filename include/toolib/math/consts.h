// 2016-17

//! \file

#ifndef CONSTS_H_sjkhnfxgiuexernhgf7834t4
#define CONSTS_H_sjkhnfxgiuexernhgf7834t4

#include "../config.h"
#include "ul/ul.h"
#include <cmath>

#include "ul/macros.h"

namespace mb::too::math::consts
{
template <typename T>
constexpr T pi()
{
    return std::acos(static_cast<T>(-1));
}
} // namespace mb::too::math::consts

#include "ul/macros_end.h"

TOO_HEADER_END

#endif
