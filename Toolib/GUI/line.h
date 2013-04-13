// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

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
