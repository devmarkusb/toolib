// Copyright (c) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

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
