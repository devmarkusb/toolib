// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef PIXEL_WCONS_H_INCL
#define PIXEL_WCONS_H_INCL

#include "../pixel.h"

namespace too
{
	namespace gui
	{
		class WCONSPixel : public IPixel
		{
		public:
			WCONSPixel(const Point& p) : IPixel(p) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
