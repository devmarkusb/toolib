// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//! File serves to export symbols for Toolib as a library.
/** Note that it uses itself a little bit to do that.
*/
//! \file

#ifndef TOOLIBDEF_H_INCL_aibuxsgfnz342975ty31
#define TOOLIBDEF_H_INCL_aibuxsgfnz342975ty31

#define TOO_LINKLIB_IMPL_CHOICE     TOO_LINKLIB_IMPL_CHOICE_QT
#include "linklib/LINKLIB_DEFS.h"

#if defined(TOOLIB_LIBRARY)
#define TOOLIBSHARED_EXPORT     TOO_DECL_EXPORT
#else
#define TOOLIBSHARED_EXPORT     TOO_DECL_IMPORT
#endif

#endif
