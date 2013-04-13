// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef LINE_H_INCL
#define LINE_H_INCL

#include "shape.h"

namespace too
{
	namespace gui
	{
		class ILine : public virtual IShape
		{
		public:
			ILine(const Point& p1, const Point& p2) { addPoint(p1); addPoint(p2); }
			// default copy and assignment ok
		protected:
			virtual void draw_details() const=0;
			virtual void move_details(int dx, int dy);
		};
	}
}

#endif
