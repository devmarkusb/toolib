// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef GUI_FL_H_INCL
#define GUI_FL_H_INCL

#include "../../gui.h"
#include "gui_engine_FL.h"
#include "window_FL.h"
#include "pixel_FL.h"
#include "line_FL.h"
#include "rectangle_FL.h"
#include "text_FL.h"
#include "circle_FL.h"

namespace too
{
	namespace gui
	{
		class FL_GUILib : public IGUILib
		{
		public:
			virtual ~FL_GUILib(){}
			// default copy and assignment ok

			virtual IEngine* Engine() const { return new FLEngine(); }
			virtual IWindow* Window(int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0) const
			{
				return new FLWindow(iWidth, iHeight, sTitle);
			}
			virtual IWindow* Window(const Point& left_top, int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0) const
			{
				return new FLWindow(left_top, iWidth, iHeight, sTitle);
			}
			virtual IPixel* Pixel(const Point& p) const { return new FLPixel(p); }
			virtual ILine* Line(const Point& p1, const Point& p2) const { return new FLLine(p1, p2); }
			virtual IRectangle* Rectangle(const Point& p1, const Point& p2) const
				 { return new FLRectangle(p1, p2); }
			virtual IRectangle* Rectangle(const Point& p1, int iWidth, int iHeight) const
				 { return new FLRectangle(p1, iWidth, iHeight); }
			virtual IText* Text(const Point& p, const std::string& s) const
				 { return new FLText(p, s); }
			virtual ICircle* Circle(const Point& m, float r) const
				 { return new FLCircle(m, r); }
		};
	} // gui
} // too

#endif