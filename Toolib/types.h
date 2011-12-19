// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef TOOTYPES_H_INCL_com3iur982zxr920z427
#define TOOTYPES_H_INCL_com3iur982zxr920z427

#include "PPDEFS.H"

namespace too
{

#if TOO_MS_VISUAL_STUDIO_CPP
	typedef unsigned __int8		u8;
	typedef __int8				s8;
	typedef unsigned __int16	u16;
	typedef __int16				s16;
	typedef unsigned __int32	u32;
	typedef __int32				s32;
#else
	typedef unsigned char		u8;
	typedef signed char			s8;
	typedef unsigned short		u16;
	typedef signed short		s16;
	typedef unsigned int		u32;
	typedef signed int			s32;
#endif

	typedef float				f32;
	typedef double				f64;
	typedef long double			f128;

	typedef unsigned char		uchar;
	typedef unsigned char		byte;
	typedef signed char			schar;
	typedef unsigned short		ushort;
	typedef signed short		sshort;
	typedef unsigned int		uint;
	typedef signed int			sint;

	typedef unsigned long		ulong;
	typedef signed long			slong;
}

#endif