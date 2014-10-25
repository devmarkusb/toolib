// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#include "Toolib/GUI/WCONS/window_WCONS.h"
#include "Toolib/cons_w32/cons_w32.h"
#include "Toolib/GUI/WCONS/color_conv_WCONS.h"
#include "Toolib/str2wstr.h"

namespace too
{
	namespace gui
	{
		void WCONSWindow::init(int iWidth, int iHeight, const std::string& sTitle)
		{
			WCONS::color_conv_ic::init();
			con::con.setWndSize(iWidth, iHeight);
			con::con.hide();
			con::con.setCurSize(0);
			con::con.setTitle(convert::s2ws(sTitle));
			con::con.clear();
			WCONSEngine::m_mainwindow = this;
		}
		WCONSWindow::WCONSWindow(int iWidth, int iHeight, const std::string& sTitle, IControl* parent)
			: IWindow(parent, sTitle), m_bVisible(false), m_bFullscreen(false)
		{
			init(iWidth, iHeight, sTitle);
		}
		WCONSWindow::WCONSWindow(const Point& left_top, int iWidth, int iHeight, const std::string& sTitle, IControl* parent)
			: IWindow(parent, sTitle), m_bVisible(false), m_bFullscreen(false)
		{
			init(iWidth, iHeight, sTitle);
		}
		int WCONSWindow::x() const { return con::con.getWndPosX(); }
		int WCONSWindow::y() const { return con::con.getWndPosY(); }
		int WCONSWindow::w() const { return con::con.getWndSizeX(); }
		int WCONSWindow::h() const { return con::con.getWndSizeY(); }
		void WCONSWindow::show() { con::con.show(); m_bVisible = true; }
		void WCONSWindow::hide() { con::con.hide(); m_bVisible = false; }
		bool WCONSWindow::visible() const { return m_bVisible; }
		void WCONSWindow::activate() {} //todo
		void WCONSWindow::deactivate() {} //todo
		bool WCONSWindow::active() const { return true; } //todo
		void WCONSWindow::datachange() {}
		void WCONSWindow::dataunchange() {}
		bool WCONSWindow::datachanged() const { return false; }
		void WCONSWindow::BgColor(const Color& c)
		{
			con::con.setBgColor(WCONS::color_conv_ic::toogui2ic_bg(c.get()));
			//con::con.clear();
			//con::con.CopyDblBuffer2Screen(12);
		}
		Color WCONSWindow::BgColor() const { return WCONS::color_conv_ic::ic2toogui_bg(con::con.getBgColor()); }
		int WCONSWindow::handle(Event::EType e)
		{
			int ret = 0;
			if (!(ret = handle_events(e)))
				return 0;//Fl_Double_Window::handle(e);
			return ret;
		}
		void WCONSWindow::fullscreen()
		{
			if (m_bFullscreen)
				return;
			m_dims_before_fullscreen.x = x();
			m_dims_before_fullscreen.y = y();
			m_dims_before_fullscreen.w = w();
			m_dims_before_fullscreen.h = h();
			con::con.enableWndFSMode();
			m_bFullscreen = true;
		}
		void WCONSWindow::fullscreen_off()
		{
			if (!m_bFullscreen)
				return;
			con::con.disableWndFSMode();
			m_bFullscreen = false;
		}
		void WCONSWindow::redraw()
		{
			draw();
		}
		void WCONSWindow::draw()
		{
			con::con.clear();
			IWindow::draw();
			con::con.CopyDblBuffer2Screen(5);
		}
	} // gui
} // too
