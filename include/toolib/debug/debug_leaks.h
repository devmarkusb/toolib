// 2011-16

/** \file Contains memory leak detection, wrapping just the standard procedure of Visual Studio.
If you want to use vld.h instead, if available, just define UL_USE_VLD in front of the header inclusion.
But I would rather suggest to use VLD directly. With version 2.5 it's gotten extremely simple to use.
You just need one include and one lib to link statically.*/

#ifndef DEBUG_LEAKS_H_dj823xfz23xzfn
#define DEBUG_LEAKS_H_dj823xfz23xzfn

#include "ul/macros.h"


//! Memory leak detection. Include the header as the last one (!) in your main module and use UL_DUMP_MEM_LEAKS.
/** Detection goes either by standard Visual Studio style or by using vld.h, if available, whereas
one would have to define UL_USE_VLD to 1 in front of the header inclusion.
In the standard case, one has to call UL_DUMP_MEM_LEAKS; before the program exits in order
to find leak informations in the Output window of the Visual Studio IDE.*/
#if UL_USE_VLD
#include <vld.h>
#define UL_DUMP_MEM_LEAKS 0
#else
#if UL_DEBUG && UL_COMP_MS_VISUAL_STUDIO_CPP && (UL_MS_VS_VER > 1299)
#define CRTDBG_MAP_ALLOC
#define _CRTDBG_MAP_ALLOC
#define DEBUG_CLIENTBLOCK new (_CLIENT_BLOCK, __FILE__, __LINE__)
#include <crtdbg.h>
#include <stdlib.h>
#define new               DEBUG_CLIENTBLOCK
#define UL_DUMP_MEM_LEAKS _CrtDumpMemoryLeaks()
#else
#define UL_DUMP_MEM_LEAKS 0
#endif
#endif // UL_USE_VLD

#include "ul/macros_end.h"

#endif
