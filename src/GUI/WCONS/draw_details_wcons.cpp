// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#include <iostream>
#include <string>
#include <algorithm>
#include "Toolib/GUI/WCONS/pixel_WCONS.h"
#include "Toolib/GUI/WCONS/line_WCONS.h"
#include "Toolib/GUI/WCONS/rectangle_WCONS.h"
#include "Toolib/GUI/WCONS/text_WCONS.h"
#include "Toolib/GUI/WCONS/circle_WCONS.h"
#include "Toolib/math/round.h"
#include "Toolib/cons_w32/cons_w32.h"
#include "Toolib/GUI/WCONS/color_conv_WCONS.h"
#include "Toolib/math/vector.h"

namespace too
{
	namespace gui
	{
		namespace WCONS
		{
			namespace draw_details
			{
				inline void rect(int x1, int y1, int w, int h)
				{
					std::string s(w+1, char(219));
					con::con.puts(x1, y1, s);
					con::con.puts(x1, y1+h, s);
					for (int i = 1; i < h; ++i)
					{
						con::con.puts(x1, y1+i, std::string(1, char(219)));
						con::con.puts(x1+w, y1+i, std::string(1, char(219)));
					}
				}
				inline void rectf(int x1, int y1, int w, int h)
				{
					std::string s(w+1, char(219));
					for (int i = 0; i <= h; ++i)
					{
						con::con.puts(x1, y1+i, s);
					}
				}
			}
		}

		void WCONSPixel::draw_details() const
		{
			if (!FgColor().visibility())
				return;
			const Point& p = getPoint(0);
			Color::EType c = FgColor().get();
			con::con.setTextColor(WCONS::color_conv_ic::toogui2ic(c));
			con::con.puts(p.x, p.y, std::string(1, char(219)));
		}

		void WCONSLine::draw_details() const
		{
			if (!FgColor().visibility())
				return;
			const Point& p1 = getPoint(0);
			const Point& p2 = getPoint(1);
			Color::EType c = FgColor().get();
			con::con.setTextColor(WCONS::color_conv_ic::toogui2ic(c));
//#define MY_OWN_DRAW_DETAILS_ALG
#ifdef MY_OWN_WCONSLINE_DRAW_DETAILS_ALG
			con::con.puts(p1.x, p1.y, std::string(1, char(219)));
			con::con.puts(p2.x, p2.y, std::string(1, char(219)));
			math::vector2dd v1(p1.x, p1.y);
			math::vector2dd v2(p2.x, p2.y);
			math::vector2dd vd(v2-v1);
			int iSteps = math::round2<int>(vd.length()) - 1;
			double dInterv = 1.0;
			dInterv/= iSteps;
			int x, y;
			for (int i = 1; i <= iSteps; ++i)
			{
				x = math::round2<int>(p1.x + dInterv*i*(p2.x-p1.x));
				y = math::round2<int>(p1.y + dInterv*i*(p2.y-p1.y));
				con::con.puts(x, y, std::string(1, char(219)));
			}
#else
			int x1 = p1.x, y1 = p1.y, x2 = p2.x, y2 = p2.y;
			int deltax = abs(x2 - x1);
			int deltay = abs(y2 - y1);
			int x = x1;
			int y = y1;
			int xinc1;
			int xinc2;
			int yinc1;
			int yinc2;
			int den;
			int num;
			int numadd;
			int numpixels;

			if (x2>=x1) {
				xinc1 = 1;
				xinc2 = 1;
			} else {
				xinc1 = -1;
				xinc2 = -1;
			}

			if (y2>=y1) {
				yinc1 = 1;
				yinc2 = 1;
			} else {
				yinc1 = -1;
				yinc2 = -1;
			}

			if (deltax>=deltay) {
				xinc1 = 0;
				yinc2 = 0;
				den = deltax;
				num = deltax / 2;
				numadd = deltay;
				numpixels = deltax;
			} else {
				xinc2 = 0;
				yinc1 = 0;
				den = deltay;
				num = deltay / 2;
				numadd = deltax;
				numpixels = deltay;
			}

			for (int curpixel=0; curpixel<=numpixels; curpixel++) {
				//if (x>=0 && x<80 && y>=0 && y<50) chars[x+y*80] = colour;
				con::con.puts(x, y, std::string(1, char(219)));
				num += numadd;
				if (num >= den) {
					num -= den;
					x += xinc1;
					y += yinc1;
				}
				x += xinc2;
				y += yinc2;
			}
#endif
		}

		void WCONSRectangle::draw_details() const
		{
			const Point& p = getPoint(0);
			const Point& wh = getPoint(1);
			const Color& f = FillColor();
			const Color& fg = FgColor();
			if (f.visibility())
			{
				Color::EType c = f.get();
				con::con.setTextColor(WCONS::color_conv_ic::toogui2ic(c));
				WCONS::draw_details::rectf(p.x, p.y, wh.x, wh.y);
			}
			if (fg.visibility())
			{
				Color::EType c = fg.get();
				con::con.setTextColor(WCONS::color_conv_ic::toogui2ic(c));
				WCONS::draw_details::rect(p.x, p.y, wh.x, wh.y);
			}
		}

		void WCONSText::draw_details() const
		{
			if (!FgColor().visibility())
				return;
			const Point& p = getPoint(0);
			const std::string& s = get();
			Color::EType c = FgColor().get();
			con::con.setTextColor(WCONS::color_conv_ic::toogui2ic(c));
			//con::con.setCurPos(p.x, p.y);
			//std::cout << s;
			con::con.puts(p.x, p.y, s);
		}

		void WCONSCircle::draw_details() const
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
				//fl_color(f.as_int());
				//fl_pie(p.x, p.y, i2r, i2r, 0.0, 360.0);
			}
			if (fg.visibility())
			{
				//fl_color(fg.as_int());
				//fl_arc(p.x, p.y, i2r, i2r, 0.0, 360.0);
			}
		}
	} // gui
} // too
