// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#include <sstream>
#include "Toolib/cons_w32/catch_exit.h"
#include "Toolib/log.h"
#include "Toolib/LOGDEF.h"

namespace too
{
	namespace con
	{
		bool ExitCatcher::m_bRequestedExit = false;

		BOOL ExitCatcher::CtrlHandler(DWORD fdwCtrlType) 
		{ 
			switch(fdwCtrlType) 
			{ 
			case CTRL_C_EVENT: 
				TOOLOGi("Ctrl-C event.");
				m_bRequestedExit = true;
				return(TRUE);
			case CTRL_CLOSE_EVENT: 
				TOOLOGi("Ctrl-Close event.");
				m_bRequestedExit = true;
				return(TRUE); 
			case CTRL_BREAK_EVENT: 
				TOOLOGi("Ctrl-Break event.");
				m_bRequestedExit = true;
				return FALSE; 
			case CTRL_LOGOFF_EVENT: 
				TOOLOGi("Ctrl-Logoff event.");
				m_bRequestedExit = true;
				return FALSE; 
			case CTRL_SHUTDOWN_EVENT: 
				m_bRequestedExit = true;
				return FALSE; 
			default: 
				TOOLOGi("Ctrl-? event.");
				m_bRequestedExit = true;
				return FALSE;
			} 
		}

		ExitCatcher::ExitCatcher()
		{
			if(!SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(CtrlHandler), TRUE))
			{
				std::ostringstream os;
				os << "W32ConsExitCatcher: SetConsoleCtrlHandler() error: " << GetLastError();
				TOOLOGe(os.str());
			}
		}

		ExitCatcher& ExitCatcher::getInstance()
		{
			static ExitCatcher ec;
			return ec;
		}

		ExitCatcher& W32ConsExitCatcher = ExitCatcher::getInstance();
	} // con
} // too
