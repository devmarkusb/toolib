// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef POINT_H_INCL
#define POINT_H_INCL

namespace too
{
	namespace gui
	{
		struct Point
		{
			int x, y;
			Point(int x0, int y0) : x(x0), y(y0){}
			Point() : x(0), y(0){}
		};

		inline bool operator==(const Point& a, const Point& b) { return a.x==b.x && a.y==b.y; }
		inline bool operator!=(const Point& a, const Point& b) { return !(a==b); }
	}
}

#endif
