/** \file File serves to export symbols for toolib as a library.
    Note that it uses itself a little bit to do that.*/

#ifndef TOOLIBDEF_H_AIBUXSGFNZ342975TY31
#define TOOLIBDEF_H_AIBUXSGFNZ342975TY31

#include "ul/linklib/LINKLIB_DEFS.h"


#if defined(TOOLIB_LIBRARY)
#define TOOLIBSHARED_EXPORT UL_DECL_EXPORT
#else
#define TOOLIBSHARED_EXPORT UL_DECL_IMPORT
#endif

#endif
