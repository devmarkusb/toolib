// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef CONSTS_H_INCL_sjkhnfxgiuexernhgf7834t4
#define CONSTS_H_INCL_sjkhnfxgiuexernhgf7834t4

#include "Toolib/PPDEFS.h"
#include "Toolib\std\std_extensions.h"
#include <cmath>


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
}
}
}

#endif
