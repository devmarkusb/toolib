// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

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
