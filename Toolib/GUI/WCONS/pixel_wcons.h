// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

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
