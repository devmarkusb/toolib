// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef RECTANGLE_H_INCL
#define RECTANGLE_H_INCL

#include "shape.h"

namespace too
{
	namespace gui
	{
		class IRectangle : public virtual IShape
		{
		public:
			IRectangle(const Point& p1, const Point& p2) { addPoint(p1); addPoint(Point(p2.x-p1.x, p2.y-p1.y)); }
			IRectangle(const Point& p1, int w, int h) { addPoint(p1); addPoint(Point(w, h)); }
			// default copy and assignment ok
		protected:
			virtual void draw_details() const=0;
			virtual void move_details(int dx, int dy);
		};
	}
}

#endif
