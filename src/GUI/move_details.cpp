// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#include "Toolib/GUI/pixel.h"
#include "Toolib/GUI/line.h"
#include "Toolib/GUI/rectangle.h"
#include "Toolib/GUI/text.h"
#include "Toolib/GUI/circle.h"

namespace too
{
	namespace gui
	{
		void IPixel::move_details(int dx, int dy)
		{
			Point& p = getPoint(0);
			p.x+= dx; p.y+= dy;
		}

		void ILine::move_details(int dx, int dy)
		{
			Point& p1 = getPoint(0);
			Point& p2 = getPoint(1);
			p1.x+= dx; p1.y+= dy;
			p2.x+= dx; p2.y+= dy;
		}

		void IRectangle::move_details(int dx, int dy)
		{
			Point& p = getPoint(0);
			p.x+= dx; p.y+= dy;
		}

		void IText::move_details(int dx, int dy)
		{
			Point& p = getPoint(0);
			p.x+= dx; p.y+= dy;
		}

		void ICircle::move_details(int dx, int dy)
		{
			Point& p = getPoint(0);
			p.x+= dx; p.y+= dy;
		}
	} // gui
} // too
