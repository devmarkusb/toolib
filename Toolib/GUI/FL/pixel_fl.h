// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef PIXEL_FL_H_INCL
#define PIXEL_FL_H_INCL

#include "../pixel.h"

namespace too
{
	namespace gui
	{
		class FLPixel : public IPixel
		{
		public:
			FLPixel(const Point& p) : IPixel(p) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
