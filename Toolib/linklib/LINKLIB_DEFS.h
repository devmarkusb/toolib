// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/** Usage I: The following steps are for a library (shared or static) as intended target.
    (1) In the build configuration of your library define sth. like YOURLIB_LIBRARY. This is
    the indicator, that a library should be built from the code. Of course you don't do this in
    the application which later uses the library.
    (2) Within your library code base create your own YourlibDEF.h among the shared include subdir
    which includes this file here and place your own library import/export define in there like
    \code
    // ...don't forget include guard...
    #include "Toolib/linklib/LINKLIB_DEFS.h"
    #if defined(YOURLIB_LIBRARY)
    #define YOURLIBSHARED_EXPORT    TOO_DECL_EXPORT
    #else
    #define YOURLIBSHARED_EXPORT    TOO_DECL_IMPORT
    #endif
    \endcode
    // ...
    (3) Include your YourlibDEF.h into all your library headers that export things and ought to be used
    from the outside, too. Of course, if you have only one header to share, you can put the code of YourlibDEF.h
    right into there and spare that additional header.
    (4) Exporting things then goes like this:
	\code
    #include "YourlibDEF.h"
    class YOURLIBSHARED_EXPORT CExample {...};
    YOURLIBSHARED_EXPORT inline void f(...) {...}
    YOURLIBSHARED_EXPORT void g(...);
    YOURLIBSHARED_EXPORT int i;
	\endcode
    (5) Remark: Do not use a DllMain function. This is Windows-only.
    (6) Important additional configuration of this LINKLIB_DEFS.h:
    Right before including this header in your own YourlibDEF.h, put in a define assignment as follows
    (as long as you don't want the default value)
    \code
    // ...don't forget include guard...
    #define TOO_LINKLIB_IMPL_CHOICE         TOO_LINKLIB_IMPL_CHOICE_xxx
    #include "Toolib/linklib/LINKLIB_DEFS.h"
    // ...
    \endcode
    where xxx is one of
    DEFAULT or equally TOOLIB
    QT
    Note that the TOOLIB choice is Windows only so far.
    (7) Add TOO_EXTERN_C_DECLS (next to YOURLIBSHARED_EXPORT) if you intend to allow your library users the
    choice of late binding.

    Usage II: The following documentation is for application code which likes to call or link your library.
    - Just linking (statically or early binding).
        Nothing to do. Just include your library headers with the exported symbols that you like to use.
    - Late binding (loading a library on your own at an arbitrary point during run-time)
        1. Include the wanted library headers like above also. Do not include this file LINKLIB_DEFS.h directly,
        because you definitely want to use the same TOO_LINKLIB_IMPL_CHOICE for lib and user, trust me.
        2. Make sure the library header exports its symbols with TOO_EXTERN_C_DECLS.
        To some extend it seems to be even possible to export namespace content and STL stuff.
        If there is no TOO_EXTERN_C_DECLS, the symbol cannot be binded late.
        3. Use ILibrary from ilibrary.h (automatically included with this file, if specific implementation supported
        at all) by starting with make() function call, which already tries to load the library if a file name
        is provided.
        4. Note: use function pointers for symbol resolving. A redirection via std::function doesn't help much, since
        there is no equivalence std::function* <-> function pointer.
    - Note: Never use late binding, if not absolutely necessary. There are just more pitfalls.
*/
//! \file

#ifndef LINKLIB_DEFS_H_INCL_kjdsnfxiwzgfny38tgn38g
#define LINKLIB_DEFS_H_INCL_kjdsnfxiwzgfny38tgn38g

#include "../PPDEFS.H"


//##############################################################################################################

//! The pool of choices of implementations. Use it by defining TOO_LINKLIB_IMPL_CHOICE to be one of them.
#define TOO_LINKLIB_IMPL_CHOICE_TOOLIB      1
#define TOO_LINKLIB_IMPL_CHOICE_QT          2
#define TOO_LINKLIB_IMPL_CHOICE_WX          3
#define TOO_LINKLIB_IMPL_CHOICE_DEFAULT     TOO_LINKLIB_IMPL_CHOICE_TOOLIB


//##############################################################################################################

#if !defined(TOO_LINKLIB_IMPL_CHOICE) || (TOO_LINKLIB_IMPL_CHOICE == TOO_LINKLIB_IMPL_CHOICE_TOOLIB)
#include "impl_too/LINKLIB_DEFS_too.h"
#elif TOO_LINKLIB_IMPL_CHOICE == TOO_LINKLIB_IMPL_CHOICE_QT
#include "impl_Qt/LINKLIB_DEFS_Qt.h"
#elif TOO_LINKLIB_IMPL_CHOICE == TOO_LINKLIB_IMPL_CHOICE_WX
//todo
#endif


//##############################################################################################################

//! Put TOO_EXTERN_C_DECLS in front of your YOURLIBSHARED_EXPORT-define
//! (being switch-wise TOO_DECL_EXPORT/TOO_DECL_IMPORT) if appropriate.
#if TOO_LANG_STANDARD_CPP == 1
#define TOO_EXTERN_C_DECLS        extern "C"
#define TOO_EXTERN_C_DECLS_BEGIN  extern "C" {
#define TOO_EXTERN_C_DECLS_END    }
#else
#define TOO_EXTERN_C_DECLS
#define TOO_EXTERN_C_DECLS_BEGIN
#define TOO_EXTERN_C_DECLS_END
#endif


//##############################################################################################################

#endif
