// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** You can define TOOLOG_NO_LOGGING as global compiler setting to overread all (#define-)logging commands
    and initializations (and includes and implementations).
*/
//! \file

#ifndef LOGDEF_H_INCL_fvuio45zui45zv
#define LOGDEF_H_INCL_fvuio45zui45zv

#ifndef TOOLOG_NO_LOGGING
//! Fatal error.
#define TOOLOGf(sMessage) toolog::log().putF((sMessage), __FILE__)
//! Error.
#define TOOLOGe(sMessage) toolog::log().putE((sMessage), __FILE__)
//! Warning.
#define TOOLOGw(sMessage) toolog::log().putW((sMessage))
//! Information.
#define TOOLOGi(sMessage) toolog::log().putI((sMessage))
//! Even more.
#define TOOLOGa(sMessage) toolog::log().putA((sMessage))
//! Set log file name.
#define TOOLOG_SET_LOG_FILENAME(fn) std::string toolog::logger::FILENAME = (fn)
//! Set initial tracing level.
#define TOOLOG_SET_LOG_TRACELEVEL(tl) toolog::ETraceLevel toolog::logger::TRACELEVEL = toolog::tl
//! Intro.
#define TOOLOGintroMsg(sMessage) toolog::log().putIntro((sMessage))
#define TOOLOGintro toolog::log().putIntro()
//! Outro.
#define TOOLOGoutroMsg(sMessage) toolog::log().putOutro((sMessage))
#define TOOLOGoutro toolog::log().putOutro()
// some evil extension (should be improved sometime)
#if TOOLOG_USE_STREAM_MACROS && !defined(TOOLOG_NO_LOGGING)
#define TOOLOGf_(sMessage) do { (toolog::log().putF_<<sMessage); toolog::log().putF_flush(); } while(0)
#else
#define TOOLOGf_(sMessage)
#endif
#else // TOOLOG_NO_LOGGING
#define TOOLOGf(sMessage)				sizeof (sMessage)
#define TOOLOGe(sMessage)				sizeof (sMessage)
#define TOOLOGw(sMessage)				sizeof (sMessage)
#define TOOLOGi(sMessage)				sizeof (sMessage)
#define TOOLOGa(sMessage)				sizeof (sMessage)
#define TOOLOG_SET_LOG_FILENAME(fn)		std::string toolog::logger::FILENAME = ""
#define TOOLOG_SET_LOG_TRACELEVEL(tl)	toolog::ETraceLevel toolog::logger::TRACELEVEL = toolog::TL_NO
#define TOOLOGintroMsg(sMessage)		sizeof (sMessage)
#define TOOLOGintro						0
#define TOOLOGoutroMsg(sMessage)		sizeof (sMessage)
#define TOOLOGoutro						0
#endif // TOOLOG_NO_LOGGING

#endif
