//! File serves to export symbols for the library.
//! \file

#ifndef LIBNAMEDEF_H_suanfgxz8wgfx873g4631g89f4g31g
#define LIBNAMEDEF_H_suanfgxz8wgfx873g4631g89f4g31g

#include "../../../../include/Toolib/linklib/LINKLIB_DEFS.h"

#if defined(LIBNAME_LIBRARY)
#define LIBNAMESHARED_EXPORT TOO_DECL_EXPORT
#else
#define LIBNAMESHARED_EXPORT TOO_DECL_IMPORT
#endif

#endif
