// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** Contains memory leak detection, wrapping just the standard procedure of Visual Studio.
If you want to use vld.h instead, if available, just define TOO_USE_VLD in front of the header inclusion.
*/
//! \file


#ifndef DEBUG_LEAKS_H_INCL_dj823xfz23xzfn
#define DEBUG_LEAKS_H_INCL_dj823xfz23xzfn

#include "PPDEFS.h"

//! Memory leak detection. Include the header as the last one (!) in your main module and use TOO_DUMP_MEM_LEAKS.
/** Detection goes either by standard Visual Studio style or by using vld.h, if available, whereas
one would have to define TOO_USE_VLD to 1 in front of the header inclusion (also needs dbghelp.dll
of a recent version to reside in the exe-folder).
In the standard case, one has to call TOO_DUMP_MEM_LEAKS; before the program exits in order
to find leak informations in the Output window of the Visual Studio IDE.*/
#if TOO_USE_VLD
	#include <vld.h>
	#define TOO_DUMP_MEM_LEAKS		0
#else
	#if TOO_DEBUG && TOO_MS_VISUAL_STUDIO_CPP && (TOO_MS_VS_VER > 1299)
		#define CRTDBG_MAP_ALLOC
		#define _CRTDBG_MAP_ALLOC
		#define DEBUG_CLIENTBLOCK new(_CLIENT_BLOCK, __FILE__, __LINE__)
		#include <stdlib.h>
		#include <crtdbg.h>
		#define new DEBUG_CLIENTBLOCK
		#define TOO_DUMP_MEM_LEAKS	_CrtDumpMemoryLeaks()
	#else
		#define TOO_DUMP_MEM_LEAKS	0
	#endif
#endif // TOO_USE_VLD

#endif
