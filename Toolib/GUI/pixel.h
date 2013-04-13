// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef PIXEL_H_INCL
#define PIXEL_H_INCL

#include "shape.h"

namespace too
{
	namespace gui
	{
		class IPixel : public virtual IShape
		{
		public:
			IPixel(const Point& p) { addPoint(p); }
			// default copy and assignment ok
		protected:
			virtual void draw_details() const=0;
			virtual void move_details(int dx, int dy);
		};
	}
}

#endif
