// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef LINE_WCONS_H_INCL
#define LINE_WCONS_H_INCL

#include "../line.h"

namespace too
{
	namespace gui
	{
		class WCONSLine : public ILine
		{
		public:
			WCONSLine(const Point& p1, const Point& p2) : ILine(p1, p2) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
