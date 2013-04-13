// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef SHAPE_H_INCL
#define SHAPE_H_INCL

#include <vector>
#include "point.h"
#include "color.h"

namespace too
{
	namespace gui
	{
		class IShape
		{
		public:
			virtual ~IShape(){}
			// default copy and assignment ok

			void draw() { draw_details(); }
			void move(int dx, int dy) { move_details(dx, dy); }
			//! Foreground.
			void FgColor(const Color& c) { m_fgcolor = c; }
			//! Foreground.
			const Color& FgColor() const { return m_fgcolor; }
			void FillColor(const Color& c) { m_fillcolor = c; }
			const Color& FillColor() const { return m_fillcolor; }
			void SetColors(const Color& fg, const Color& fill)  { m_fgcolor = fg; m_fillcolor = fill; }
		protected:
			void addPoint(const Point& p) { m_points.push_back(p); }
			const Point& getPoint(int i) const { return m_points[i]; }
			Point& getPoint(int i) { return m_points[i]; }

			virtual void draw_details() const=0;
			virtual void move_details(int dx, int dy)=0;
		private:
			std::vector<Point> m_points;
			Color m_fgcolor; // foreground
			Color m_fillcolor;
		};
	}
}

#endif
