// Markus Borris, 2013
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh
#define TRACE_H_INCL_dfsgjn854gcnz782x5g7813sdyfwh

#include "PPDEFS.h"
#include <sstream>
#ifdef TOO_WINDOWS
#include <windows.h>
#endif TOO_WINDOWS

namespace too
{
#ifdef TOO_WINDOWS
	//! Usage: trace(makestr() << "bla" << 2 << "blabla");
	void trace(const std::ostringstream& os)
	{
		OutputDebugString(os.str().c_str());
	}

	void trace(const std::wostringstream& os)
	{
		OutputDebugStringW(os.str().c_str());
	}
#endif TOO_WINDOWS
}

#include "string/makestr.h"

#endif
