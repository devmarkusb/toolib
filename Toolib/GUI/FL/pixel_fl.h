// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
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