// Markus Borris, 2016-17
// This file is part of toolib library.

//!
/**
 */
//! \file

#ifndef CONSTS_H_sjkhnfxgiuexernhgf7834t4
#define CONSTS_H_sjkhnfxgiuexernhgf7834t4

#include "toolib/std/std_extensions.h"
#include <cmath>

#include "toolib/macros.h"


namespace too
{
namespace math
{
namespace consts
{

template <typename T>
constexpr T pi()
{
    return std::acos(static_cast<T>(-1));
}
} // namespace consts
} // namespace math
} // namespace too

#endif
