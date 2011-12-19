// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef WINDOW_FL_H_INCL
#define WINDOW_FL_H_INCL

//#include <FL/Fl_Window.h>
#include <FL/Fl_Double_Window.h>
#include "../window.h"
#include "../point.h"

namespace too
{
	namespace gui
	{
		struct Point;

		class FLWindow : public IWindow, protected Fl_Double_Window
		{
		public:
			FLWindow(int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0)
				: IWindow(parent, sTitle),
				Fl_Double_Window(iWidth, iHeight),
				m_bFullscreen(false)
			{ copy_label(sTitle.c_str()); }
			FLWindow(const Point& left_top, int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0)
				: IWindow(parent, sTitle),
				Fl_Double_Window(left_top.x, left_top.y, iWidth, iHeight),
				m_bFullscreen(false)
			{ copy_label(sTitle.c_str()); }
			virtual ~FLWindow(){}
			// default copy and assignment forbidden for base

			virtual int x() const;
			virtual int y() const;
			virtual int w() const;
			virtual int h() const;
			virtual void show();
			virtual void hide();
			virtual bool visible() const;
			virtual void activate();
			virtual void deactivate();
			virtual bool active() const;

			virtual void datachange();
			virtual void dataunchange();
			virtual bool datachanged() const;

			virtual void BgColor(const Color& c);
			virtual Color BgColor() const;

			virtual void fullscreen();
			virtual void fullscreen_off();

			virtual int handle(int e);
		protected:
			virtual void draw();
		private:
			bool m_bFullscreen;
			struct { int x, y, w, h; } m_dims_before_fullscreen;
		};
	} // gui
} // too

#endif