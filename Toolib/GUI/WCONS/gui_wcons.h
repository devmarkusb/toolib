// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef GUI_WCONS_H_INCL
#define GUI_WCONS_H_INCL

#include "../../gui.h"
#include "gui_engine_WCONS.h"
#include "window_WCONS.h"
#include "pixel_WCONS.h"
#include "line_WCONS.h"
#include "rectangle_WCONS.h"
#include "text_WCONS.h"
#include "circle_WCONS.h"

namespace too
{
	namespace gui
	{
		class WCONS_GUILib : public IGUILib
		{
		public:
			virtual ~WCONS_GUILib(){}
			// default copy and assignment ok

			virtual IEngine* Engine() const { return new WCONSEngine(); }
			virtual IWindow* Window(int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0) const
			{
				return new WCONSWindow(iWidth, iHeight, sTitle);
			}
			virtual IWindow* Window(const Point& left_top, int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0) const
			{
				return new WCONSWindow(left_top, iWidth, iHeight, sTitle);
			}
			virtual IPixel* Pixel(const Point& p) const { return new WCONSPixel(p); }
			virtual ILine* Line(const Point& p1, const Point& p2) const { return new WCONSLine(p1, p2); }
			virtual IRectangle* Rectangle(const Point& p1, const Point& p2) const
				 { return new WCONSRectangle(p1, p2); }
			virtual IRectangle* Rectangle(const Point& p1, int iWidth, int iHeight) const
				 { return new WCONSRectangle(p1, iWidth, iHeight); }
			virtual IText* Text(const Point& p, const std::string& s) const
				 { return new WCONSText(p, s); }
			virtual ICircle* Circle(const Point& m, float r) const
				 { return new WCONSCircle(m, r); }
		};
	} // gui
} // too

#endif
