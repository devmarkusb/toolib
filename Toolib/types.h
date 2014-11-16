// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef TOOTYPES_H_INCL_com3iur982zxr920z427
#define TOOTYPES_H_INCL_com3iur982zxr920z427

#include <string>
#include "PPDEFS.H"

namespace too
{
    //! String type.
    /** Two choices are to be considered
        1. which type out of char==char8_t, wchar_t, char16_t, char32_t
            (Here you have to notice, that wchar_t is the worst choice, because it doesn't have platform independent
            fixed size, whereas char is always 8 Bit, and the other are 16, 32 Bit respectively.
        2. which encoding out of local (whatever that may be, perhaps ISO-8859-1 Latin 1 on german systems),
            wide character (whatever that superimposes on the local whatever-thing, but usually UTF-16 or UTF-32),
            UTF-8, UTF-16, UTF-32
            (Here you should notice that UTF-8 seems to be to most common choice on the internet.)

        So, what's your choice for the time being?
        Decision (to updated...):
            char and UTF-8
        Downside (always inevitable, but looking for the lesser of two evils):
            i) Convert to UTF16/32 encoding and suitable type whenever appropriate for languages,
                or frameworks/libraries (e.g. Windows API would require conversions to/from UTF-16 + wchar_t)
            ii) Convert to UTF16 + char for std::fstream & Co. as long as necessary
            iii) Exclusively write english/ASCII source code files if your compiler doesn't support the u8 string literal
                declaration prefix yet*/

    #define _ENCODING_LOCAL(x)          x
    #define _ENCODING_LOCAL_WIDE(x)     L ## x
    #define _ENCODING_UTF8(x)           u8 ## x
    #define _ENCODING_UTF16(x)          u ## x
    #define _ENCODING_UTF32(x)          U ## x

    template <typename CHAR_TYPE> using stdbasicstring_chartype = std::basic_string<CHAR_TYPE>;
#ifndef char8_t
    typedef char char8_t;
#endif
#if TOO_WINDOWS
//    using string = stdbasicstring_chartype<char16_t>;
//    #define _TOOSTR(x)     _ENCODING_UTF16(x)
    // try not using wstring anymore... but is u16string above a worthy alternative!?
//    using string = stdbasicstring_chartype<wchar_t>;
//    #define _TOOSTR(x)     _ENCODING_LOCAL_WIDE(x)
    // finally opt for a decision candidate
    using string = stdbasicstring_chartype<char8_t>;
    #define _TOOSTR(x)      _ENCODING_UTF8(x)
#else
    using string = stdbasicstring_chartype<char8_t>;
    #define _TOOSTR(x)      _ENCODING_UTF8(x)
#endif


    //############################################################################################################

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
