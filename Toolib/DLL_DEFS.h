// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** NOTE: This file remains just as a documentation. It doesn't make much sense. Every dll should use its own
    export define.
    Usage: The following steps are for a DLLs as intended target. The absolute same applies for a LIBs - you just
	leave out the define TOO_BUILDANDUSE_DLLS from the global compiler options. The project then remains flexible,
	whether to be compiled to DLLs or LIBs.
	(1) Include this file into all your library headers that export things and ought to be used
	from the outside, too.
	(2) Exporting for possible DLL target then goes like this:
	\code
	#include "DLL_DEFS.h"
	class TOO_DLL_DECLSPEC CExample {...};
	TOO_DLL_DECLSPEC inline void f(...) {...}
	TOO_DLL_DECLSPEC void g(...);
	TOO_DLL_DECLSPEC int i;
	\endcode
	(2) Define TOO_BUILDANDUSE_DLLS as global compiler option when compiling *and* using as DLL.
	(3) Define TOO_DLL_EXPORT if compiling the DLL, and don't define it when using it (corresponds to
	alternatively define TOO_DLL_IMPORT.
	(4) When using outside, import the DLL/LIB by
		\code
		#pragma comment(lib, "filename_of_lib_or_dll.lib")
		\endcode
	(5) If needed, your library main module (.cpp) can contain a DllMain function. A code-snippet for that
	is provided below.

	Note: So far everything is restricted to Windows (and MS Visual Studio).

	(A) Code-snippet for a DllMain:
	\code
	#include "DLL_DEFS.h"
	#ifdef TOO_WINDOWS
	#include <windows.h>
	#ifdef TOO_STANDARD_CPP_CLI
	#pragma managed(push, off)
	#endif
	BOOL APIENTRY DllMain(HMODULE hModule,
						  DWORD  ul_reason_for_call,
						  LPVOID lpReserved)
	{
		switch (ul_reason_for_call)
		{
		case DLL_PROCESS_ATTACH:
		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
		}
		return TRUE;
	}
	#ifdef TOO_STANDARD_CPP_CLI
	#pragma managed(pop)
	#endif
	#endif // TOO_WINDOWS
	\endcode
*/
//! \file

#ifndef DLL_DEFS_H_INCL_fvhnbil8rw75t
#define DLL_DEFS_H_INCL_fvhnbil8rw75t

#if THIS_DLL_IS_NOT_DEPRECATED__DLL_DEFS_H_INCL_fvhnbil8rw75t

#include "PPDEFS.H"

//##############################################################################################################

//! TOO_BUILDANDUSE_DLLS must be defined when you compile a library as DLL and also when you use it.
#if defined(TOO_BUILDANDUSE_DLLS) && TOO_WINDOWS
//! TOO_DLL_EXPORT must be defined when compiling the dll, otherwise it mustn't.
#if defined(TOO_DLL_EXPORT) && !defined(TOO_DLL_IMPORT)
//! For exporting the library to a dll.
#define TOO_DLL_DECLSPEC __declspec(dllexport)
#else
//! For using the library (dll) in a project.
#define TOO_DLL_DECLSPEC __declspec(dllimport)
#endif
#else
//! Target/use LIB or non-Windows.
#define TOO_DLL_DECLSPEC
#endif


//! Put in front of TOO_DLL_DECLSPEC if appropriate.
#define TOO_EXTERN_C extern "C"


#if defined(_STDCALL_SUPPORTED)
//! Declare the general dll calling convention.
#define TOO_DLLCALLCONV __stdcall
#else
//! Declare the general dll calling convention.
#define TOO_DLLCALLCONV __cdecl
#endif


#if TOO_WINDOWS && ((_MSC_VER >= 800) || defined(_STDCALL_SUPPORTED))
//! Declare the dll calling convention for DllMain, Windows.
#define TOO_WINAPI      __stdcall
//! Declare the dll calling convention for DllMain, Windows.
#else
#define TOO_WINAPI
#endif
#define TOO_APIENTRY    WINAPI


#endif THIS_DLL_IS_NOT_DEPRECATED__DLL_DEFS_H_INCL_fvhnbil8rw75t

//##############################################################################################################
#endif
