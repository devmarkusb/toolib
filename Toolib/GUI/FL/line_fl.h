// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef LINE_FL_H_INCL
#define LINE_FL_H_INCL

#include "../line.h"

namespace too
{
	namespace gui
	{
		class FLLine : public ILine
		{
		public:
			FLLine(const Point& p1, const Point& p2) : ILine(p1, p2) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
