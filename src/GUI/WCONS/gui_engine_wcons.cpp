// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

//#include <windows.h>
#include <algorithm>
#include <conio.h>
#include "Toolib/cons_w32/cons_w32.h"
#include "Toolib/GUI/WCONS/gui_engine_WCONS.h"
#include "Toolib/GUI/WCONS/window_WCONS.h"
#include "Toolib/GUI/key.h"
#include "Toolib/GUI/keycodes.h"
#include "Toolib/GUI/event.h"
#include "Toolib/clock.h"

namespace too
{
	namespace gui
	{
		WCONSWindow* WCONSEngine::m_mainwindow = 0;
		//bool WCONSEngine::m_bRequestTerminate = false;

		int WCONSEngine::main() { while(wait()); return 0; }
		int WCONSEngine::wait()
		{
			static clock mainclock;
			if (m_bRequestRedraw)
			{
				m_mainwindow->redraw();
				m_bRequestRedraw = false;
			}
			if (m_fpIdle)
				m_fpIdle(0);
			typedef std::vector<timeout>::iterator itTO;
			clock_t elapsed;
			for (itTO it = m_timeouts.begin(); it != m_timeouts.end(); ++it)
			{
				timeout& to = *it;
				elapsed = mainclock.elapsed(false);
				if (to.t <= elapsed-to.elapsed)
				{
					to.elapsed = elapsed;
					if (to.f)
						to.f(0);
				}
			}
			//if (_kbhit())
			if (con::con.getEvents())
			{
				if (con::con.getFirstKeyEvent(m_tKeyEvent))
				{
					m_bKeyEvent = true;
					m_mainwindow->handle(Event::E_KEYBOARD);
				}
				if (con::con.getFirstMouseClickEvent(m_tMouseEvent))
				{
					m_bMouseEvent = true;
					m_mainwindow->handle(Event::E_PUSH);
				}
			}
			if (con::W32ConsExitCatcher.RequestedExit())
			{
				//m_bRequestTerminate = false;
				return 0;
			}
			return 1;
		}
		int WCONSEngine::screen_w() const { return con::con.getMaxWndSizeX(); }
		int WCONSEngine::screen_h() const { return con::con.getMaxWndSizeY(); }
		int WCONSEngine::screen_left() const { return 0; }
		int WCONSEngine::screen_top() const { return 0; }

		void WCONSEngine::add_idle(fpCALLBACKpv cb) { m_fpIdle = cb; }
		void WCONSEngine::remove_idle(fpCALLBACKpv cb) { m_fpIdle = 0; }
		void WCONSEngine::add_timeout(double t, fpCALLBACKpv cb) { m_timeouts.push_back(timeout(t*1000.0, cb)); }
		void WCONSEngine::repeat_timeout(double t, fpCALLBACKpv cb)
		{ //todo
		}
		void WCONSEngine::remove_timeout(fpCALLBACKpv cb)
		{
			//todo wrong implementation
			if (!m_timeouts.empty())
				m_timeouts.pop_back();
		}

		int WCONSEngine::event() { return 0; } //todo
		int WCONSEngine::event_inside(int x1, int y1, int x2, int y2)
		{ return 0; } //todo
		int WCONSEngine::event_x() { return 0; } //todo
		int WCONSEngine::event_y() { return 0; } //todo
		int WCONSEngine::event_button() { return 0; } //todo
		int WCONSEngine::event_keystate() { return 0; } //todo
		int WCONSEngine::event_key()
		{
			if (m_bKeyEvent)
			{
				m_bKeyEvent = false;
				return m_tKeyEvent.wVirtualKeyCode;
				/*char k = _getch();
				char kk;
				switch (k)
				{
				case 0:
					kk =  _getch();
					return kk; //todo return sth. correctly adjusted to our key code table (key.h)
				case 0xE0:
					kk =  _getch();
					return kk; //todo return sth. correctly adjusted to our key code table (key.h)
				default:
					return k; //todo probably dito?
				}*/
			}
			return KEY_NOKEY;
		}
		int WCONSEngine::event_key(int k)
		{
			return (event_key()==k)!=0;
		}

		void WCONSEngine::flush() {} //todo

		void WCONSEngine::redraw() { m_bRequestRedraw = true; }

		void WCONSEngine::lock() {} //todo
		void WCONSEngine::unlock() {} //todo

		void WCONSEngine::initHwDblBuff()
		{
		}

		void WCONSEngine::test() {}
	} // gui
} // too
