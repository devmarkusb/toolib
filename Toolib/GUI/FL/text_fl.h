// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef TEXT_FL_H_INCL
#define TEXT_FL_H_INCL

#include "../text.h"

namespace too
{
	namespace gui
	{
		class FLText : public IText
		{
		public:
			FLText(const Point& p, const std::string& s) : IText(p, s) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
