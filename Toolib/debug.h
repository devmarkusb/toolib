// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** Contains useful tools for debugging, like run-time breakpoints and memory leak detection
(wrapping just the standard procedure of Visual Studio). If you want to use vld.h for the latter,
if available, just define TOO_USE_VLD in front of the header inclusion.
Note: For leak detection this file has to be the last one included in your main module.
If you do not want leak detection, define TOO_NO_LEAK_DETECTION in front of the inclusion.
Known issue: problems occur when you try to overload operator new after including leak detection.
*/
//! \file

#pragma once
#ifndef DEBUG_H_INCL_comwcicjhr98234nr
#define DEBUG_H_INCL_comwcicjhr98234nr

//! For further informations, see included files.
#include "debug/debug_misc.h"
#ifndef TOO_NO_LEAK_DETECTION
	#include "debug/debug_leaks.h"
#endif

#endif