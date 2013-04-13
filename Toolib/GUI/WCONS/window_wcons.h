// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef WINDOW_WCONS_H_INCL
#define WINDOW_WCONS_H_INCL

#include "../window.h"
#include "../point.h"
#include "gui_engine_wcons.h"

namespace too
{
	namespace gui
	{
		struct Point;

		class WCONSWindow : public IWindow
		{
		public:
			WCONSWindow(int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0);
			WCONSWindow(const Point& left_top, int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0);
			virtual ~WCONSWindow(){}
			// default copy and assignment forbidden for base

			//! PERHAPS: Has not the usual meaning of window position on screen, but window position w.r.t. buffer.
			virtual int x() const;
			//! PERHAPS: Has not the usual meaning of window position on screen, but window position w.r.t. buffer.
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

			virtual int handle(Event::EType e);
			virtual void redraw();
		protected:
			virtual void draw();
		private:
			bool m_bVisible;
			bool m_bFullscreen;
			struct { int x, y, w, h; } m_dims_before_fullscreen;

			void init(int iWidth, int iHeight, const std::string& sTitle);
		};
	} // gui
} // too

#endif
