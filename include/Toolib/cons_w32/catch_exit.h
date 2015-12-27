// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef CATCH_EXIT_H_INCL_34trcx83943g
#define CATCH_EXIT_H_INCL_34trcx83943g

#include <windows.h>
#include "../../ToolibDEF.h"

namespace too
{
	namespace con
	{
                class TOOLIBSHARED_EXPORT ExitCatcher
		{
		public:
			//! Use global reference W32ConsExitCatcher instead.
			static ExitCatcher& getInstance();

			bool RequestedExit()
			{
				if (m_bRequestedExit)
				{
					m_bRequestedExit = false;
					return true;
				}
				else
				{
					return false;
				}
			}
		private:
			static bool m_bRequestedExit;

			static BOOL CtrlHandler(DWORD fdwCtrlType);

			ExitCatcher();
		};

	}
}

#endif
