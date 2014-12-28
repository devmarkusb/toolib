// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef TOOLIB_QTDEF_H_INCL_kshnifusegf87w3nrtgf2y6378gfdy
#define TOOLIB_QTDEF_H_INCL_kshnifusegf87w3nrtgf2y6378gfdy

#define TOO_LINKLIB_IMPL_CHOICE     TOO_LINKLIB_IMPL_CHOICE_QT
#include "Toolib/linklib/LINKLIB_DEFS.h"

#if defined(TOOLIB_QT_LIBRARY)
#define TOOLIBSHARED_EXPORT     TOO_DECL_EXPORT
#else
#define TOOLIBSHARED_EXPORT     TOO_DECL_IMPORT
#endif

#endif
