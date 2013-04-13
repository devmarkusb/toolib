// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef CIRCLE_FL_H_INCL
#define CIRCLE_FL_H_INCL

#include "../circle.h"

namespace too
{
	namespace gui
	{
		class FLCircle : public ICircle
		{
		public:
			FLCircle(const Point& m, float r) : ICircle(m, r) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
