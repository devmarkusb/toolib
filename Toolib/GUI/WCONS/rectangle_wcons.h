// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef RECTANGLE_WCONS_H_INCL
#define RECTANGLE_WCONS_H_INCL

#include "../rectangle.h"

namespace too
{
	namespace gui
	{
		class WCONSRectangle : public IRectangle
		{
		public:
			WCONSRectangle(const Point& p1, const Point& p2) : IRectangle(p1, p2) {}
			WCONSRectangle(const Point& p1, int w, int h) : IRectangle(p1, w, h) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
