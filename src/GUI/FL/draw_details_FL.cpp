// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#include "Toolib/GUI/FL/pixel_FL.h"
#include "Toolib/GUI/FL/line_FL.h"
#include "Toolib/GUI/FL/rectangle_FL.h"
#include "Toolib/GUI/FL/text_FL.h"
#include "Toolib/GUI/FL/circle_FL.h"
#include "Toolib/math/round.h"

#include "FL/fl_draw.H"

namespace too
{
	namespace gui
	{
		void FLPixel::draw_details() const
		{
			if (!FgColor().visibility())
				return;
			const Point& p = getPoint(0);
			//Fl_Color old = fl_color();
			fl_color(FgColor().as_int());
			fl_point(p.x, p.y);
			//fl_color(old);
		}

		void FLLine::draw_details() const
		{
			if (!FgColor().visibility())
				return;
			const Point& p1 = getPoint(0);
			const Point& p2 = getPoint(1);
			//Fl_Color old = fl_color();
			fl_color(FgColor().as_int());
			fl_line(p1.x, p1.y, p2.x, p2.y);
			//fl_color(old);
		}

		void FLRectangle::draw_details() const
		{
			const Point& p = getPoint(0);
			const Point& wh = getPoint(1);
			const Color& f = FillColor();
			const Color& fg = FgColor();
			if (f.visibility())
			{
				fl_color(f.as_int());
				fl_rectf(p.x, p.y, wh.x, wh.y);
			}
			if (fg.visibility())
			{
				fl_color(fg.as_int());
				fl_rect(p.x, p.y, wh.x, wh.y);
			}
		}

		void FLText::draw_details() const
		{
			if (!FgColor().visibility())
				return;
			const Point& p = getPoint(0);
			const std::string& s = get();
			const Font& f = getFont();
			int ifs = f.getSize();
			if (ifs)
				fl_font(f.getType(), ifs);
			fl_color(FgColor().as_int());
			fl_draw(s.c_str(), p.x, p.y);
		}

		void FLCircle::draw_details() const
		{
			Point p = getPoint(0);
			const float r = Radius();
			const int ir = math::round2<int>(r);
			const int i2r = 2*ir;
			p.x-= ir; p.y-= ir;
			const Color& f = FillColor();
			const Color& fg = FgColor();
			if (f.visibility())
			{
				fl_color(f.as_int());
				fl_pie(p.x, p.y, i2r, i2r, 0.0, 360.0);
			}
			if (fg.visibility())
			{
				fl_color(fg.as_int());
				fl_arc(p.x, p.y, i2r, i2r, 0.0, 360.0);
			}
		}
	} // gui
} // too
