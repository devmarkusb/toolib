// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** You can define TOOLOG_NO_LOGGING as global compiler setting to turn logging off. This amounts to
    overreading all (#define-)logging commands and initializations (and includes and implementations).
*/
//! \file

#include "../../Toolib/log.h"

namespace too
{
	namespace logging
	{
#ifndef TOOLOG_NO_LOGGING
		bool logger::m_bSelfLog = false;
		logger& log()
		{
			static logger l;
			return l;
		}
#endif
	}
}
