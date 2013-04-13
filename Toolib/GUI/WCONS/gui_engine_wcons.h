// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef GUI_ENGINE_WCONS_H_INCL
#define GUI_ENGINE_WCONS_H_INCL

#include <vector>
#include <windows.h>
#include "../gui_engine.h"

namespace too
{
	namespace gui
	{
		class WCONSEngine : public IEngine
		{
			friend class WCONSWindow; // just that top-level Windows, when constructed, can register themselves in the engine

		public:
			WCONSEngine() : m_fpIdle(0), m_bRequestRedraw(true), m_bKeyEvent(false), m_bMouseEvent(false)
			{
				SetConsoleCtrlHandler((PHANDLER_ROUTINE)terminate, TRUE);
			}
			virtual ~WCONSEngine(){}
			// default copy and assignment ok

			virtual int main();
			virtual int wait();
			virtual int screen_w() const;
			virtual int screen_h() const;
			virtual int screen_left() const;
			virtual int screen_top() const;

			virtual void add_idle(fpCALLBACKpv cb);
			virtual void remove_idle(fpCALLBACKpv cb);
			virtual void add_timeout(double t, fpCALLBACKpv cb);
			virtual void repeat_timeout(double t, fpCALLBACKpv cb);
			//! Attention! Not yet found time to implement it the right way. Just removes the latest added timer.
			virtual void remove_timeout(fpCALLBACKpv cb);

			virtual int event();
			virtual int event_inside(int x1, int y1, int x2, int y2);
			virtual int event_x();
			virtual int event_y();
			virtual int event_button();
			virtual int event_keystate();
			virtual int event_key();
			virtual int event_key(int k);

			virtual void flush();

			virtual void redraw();

			virtual void lock();
			virtual void unlock();

			virtual void initHwDblBuff();

			virtual void test();

			//static BOOL terminate(DWORD) { m_bRequestTerminate = true; return TRUE; }
		private:
			fpCALLBACKpv m_fpIdle; // one idle function so far; could be a vector
			struct timeout
			{
				double t;
				double elapsed;
				fpCALLBACKpv f;
				timeout(double t0, fpCALLBACKpv f0) : t(t0), elapsed(0.0), f(f0){}
			};
			std::vector<timeout> m_timeouts;
			bool m_bRequestRedraw;
			static WCONSWindow* m_mainwindow; //todo allow tracking of more than one top-level window
			//static bool m_bRequestTerminate;
			bool m_bKeyEvent;
			bool m_bMouseEvent;
			KEY_EVENT_RECORD m_tKeyEvent; // undefined when m_bKeyEvent false
			MOUSE_EVENT_RECORD m_tMouseEvent; // undefined when m_bMouseEvent false
		};
	} // gui
} // too

#endif
