// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#include "Toolib/GUI/FL/window_FL.h"

namespace too
{
	namespace gui
	{
		int FLWindow::x() const { return Fl_Double_Window::x(); }
		int FLWindow::y() const { return Fl_Double_Window::y(); }
		int FLWindow::w() const { return Fl_Double_Window::w(); }
		int FLWindow::h() const { return Fl_Double_Window::h(); }
		void FLWindow::show() { Fl_Double_Window::show(); }
		void FLWindow::hide() { Fl_Double_Window::hide(); }
		bool FLWindow::visible() const { return Fl_Double_Window::visible()!=0; }
		void FLWindow::activate() { Fl_Double_Window::activate(); }
		void FLWindow::deactivate() { Fl_Double_Window::deactivate(); }
		bool FLWindow::active() const { return Fl_Double_Window::active()!=0; }
		void FLWindow::datachange() { Fl_Double_Window::set_changed(); }
		void FLWindow::dataunchange() { Fl_Double_Window::clear_changed(); }
		bool FLWindow::datachanged() const { return Fl_Double_Window::changed()!=0; }
		void FLWindow::BgColor(const Color& c) { color(c.as_int()); }
		Color FLWindow::BgColor() const { return Color(color()); }
		int FLWindow::handle(int e)
		{
			int ret = 0;
			if (!(ret = handle_events(static_cast<Event::EType>(e))))
				return Fl_Double_Window::handle(e);
			return ret;
		}
		void FLWindow::fullscreen()
		{
			if (m_bFullscreen)
				return;
			m_dims_before_fullscreen.x = x();
			m_dims_before_fullscreen.y = y();
			m_dims_before_fullscreen.w = w();
			m_dims_before_fullscreen.h = h();
			Fl_Double_Window::fullscreen();
			m_bFullscreen = true;
		}
		void FLWindow::fullscreen_off()
		{
			if (!m_bFullscreen)
				return;
			Fl_Double_Window::fullscreen_off(
				m_dims_before_fullscreen.x,
				m_dims_before_fullscreen.y,
				m_dims_before_fullscreen.w,
				m_dims_before_fullscreen.h);
			m_bFullscreen = false;
		}
		void FLWindow::draw()
		{
			Fl_Double_Window::draw();
			IWindow::draw();
		}
	} // gui
} // too
