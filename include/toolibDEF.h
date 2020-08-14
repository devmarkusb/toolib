// Markus Borris, 2014
// This file is part of my toolib library.

//! File serves to export symbols for toolib as a library.
/** Note that it uses itself a little bit to do that.
 */
//! \file

#ifndef TOOLIBDEF_H_aibuxsgfnz342975ty31
#define TOOLIBDEF_H_aibuxsgfnz342975ty31

#include "toolib/linklib/LINKLIB_DEFS.h"


#if defined(TOOLIB_LIBRARY)
#define TOOLIBSHARED_EXPORT TOO_DECL_EXPORT
#else
#define TOOLIBSHARED_EXPORT TOO_DECL_IMPORT
#endif

#endif
