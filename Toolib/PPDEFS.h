// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** Also defines the main debug macro TOO_DEBUG to be 1 or 0.
*/
//! \file

#pragma once
#ifndef PPDEFS_H_INCL_o85nzumvt424cmt75tn
#define PPDEFS_H_INCL_o85nzumvt424cmt75tn


//##############################################################################################################
// Operating Systems

#if defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__) || defined(__TOS_WIN__)
#define TOO_WINDOWS		1
#if defined(_WIN64)
#define TOO_WINDOWS_64	1
#endif
#endif

#if defined(unix) || defined(__unix__) || defined(__unix)
#define TOO_UNIX		1
#endif

#if defined(linux) || defined(__linux)
#define TOO_LINUX		1
#endif

#if defined(macintosh) || defined(Macintosh) || defined(__APPLE__) || defined(__MACH__)
#define TOO_MAC			1
#endif

#if defined(MSDOS) || defined(__MSDOS__) || defined(_MSDOS) || defined(__DOS__)
#define TOO_MSDOS		1
#endif

#if defined(_WIN32_WCE)
#define TOO_WINDOWS_CE	1
#endif


//##############################################################################################################
// Compilers

#if defined(_MSC_VER)
#define TOO_MS_VISUAL_STUDIO_CPP	1
#define TOO_MS_VS_VER				_MSC_VER
#endif

#if defined(__BORLANDC__) || defined(__CODEGEARC__)
#define TOO_BORLAND_CPP				1
#endif

#if defined(__GNUC__)
#define TOO_GNU_CPP					1
#endif


//##############################################################################################################
// Languages

#ifdef __STDC__
#define TOO_STANDARD_C			1
#define TOO_STANDARD_C89		1
#if __STDC_VERSION__ >= 199901L
#define TOO_STANDARD_C99		1
#endif
#endif

#ifdef __cplusplus
#define TOO_STANDARD_CPP		1
#define TOO_STANDARD_CPP98		1
#endif

#ifdef __cplusplus_cli
#define TOO_STANDARD_CPP_CLI	1
#define TOO_STANDARD_CPP_CLI04	1
#endif

#ifdef __embedded_cplusplus
#define TOO_STANDARD_EMBCPP		1
#endif


//##############################################################################################################
// Main debug-macro definition

#if TOO_MS_VISUAL_STUDIO_CPP
#ifdef _DEBUG
#define TOO_DEBUG	1
#else
#define TOO_DEBUG	0
#endif
#else
#ifdef NDEBUG
#define TOO_DEBUG	0
#else
#define TOO_DEBUG	1
#endif
#endif


//##############################################################################################################
// Just commenting definitions for switching on/off diverse library usages within the implementations of Toolib.
// The idea is that almost everything in Toolib is implemented by standard methods but sometimes also
// alternatively by the use of professional libraries.

#if TOO_NOT_JUST_COMMENTING_LIBRARY_SWITCH_DEFINITIONS_BUT_ACTUALLY_USE_SOME_ARBITRARY_DEFAULTS // dummy
//! Use this definition to switch on boost usage.
#define TOO_USE_BOOST 1
#endif


//##############################################################################################################
// Some common general preprocessor magic (as little as possible)

#define TOO_CONCATENATE_DIRECT(s1, s2)		s1##s2
#define TOO_CONCATENATE_INDIRECT(s1, s2)	TOO_CONCATENATE_DIRECT(s1, s2)
#define TOO_ANONYMOUS_VARIABLE(str)			TOO_CONCATENATE_INDIRECT(str, __LINE__)


//##############################################################################################################
#endif
