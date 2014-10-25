// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#include <typeinfo.h>

#include "Toolib/err.h"

namespace too
{
	namespace err
	{
		std::string getTypename(const std::exception& e)
		{
			return typeid(e).name();
		}
	} // err
} // too
