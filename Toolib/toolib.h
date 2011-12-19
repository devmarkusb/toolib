// Copyright (c) 2011 Markus Borris
// This file is part of the Toolib library.
/*
This software is provided 'as-is', without any express or implied
warranty.  In no event will the authors be held liable for any damages
arising from the use of this software.

Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it
freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not
claim that you wrote the original software. If you use this software
in a product, an acknowledgment in the product documentation would be
appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be
misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

//!
/** This is the aggregate header file of the Toolib library. It contains every header there is
(although almost never being the case realistically).
Usually this is not of any practical purpose at all. It serves just as a root of documentation
on source level.
*/
//! \file

//todo Always keep this file updated with a complete header list...

#pragma once
#ifndef TOOLIB_H_INCL_vpoi6464uifz6u45nzuz
#define TOOLIB_H_INCL_vpoi6464uifz6u45nzuz

#include <string>
#include "class_ptr.h"
#include "clock.h"
//#include "error.h"
#include "keycodes.h"
#include "log.h"
#include "matrix.h"
#include "PPDEFS.h"
#include "randomizer.h"
#include "types.h"
#include "vector.h"
//#define TOO_USE_VLD // uncomment, if you want to use vld.h, see debug.h
//#include "debug.h" // has to be included separately in your main module as the last include there is

#ifndef TOOLIB_BUILD_LIB
#pragma comment(lib, "Toolib.lib")
#endif


//!
namespace too
{
	const std::string C_S_VERSION = "1.0.0";
	const std::string C_S_1STAUTHOR = "Markus Borris";

	//!
	namespace math
	{
	}

	//!
	namespace logging
	{
	}

	//!
	namespace gui
	{
	}
}

#endif
