// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef GUI_ENGINE_FL_H_INCL
#define GUI_ENGINE_FL_H_INCL

#include <FL/Fl.H>
#include "../gui_engine.h"
#include "Toolib/log.h"
#include "Toolib/LOGDEF.h"

namespace too
{
	namespace gui
	{
		class FLEngine : public IEngine
		{
		public:
			FLEngine() { connect_loggings(); }
			virtual ~FLEngine(){}
			// default copy and assignment ok

			virtual int main() { return Fl::run(); }
			virtual int wait() { return Fl::wait(); }
			virtual int screen_w() const { return Fl::w(); }
			virtual int screen_h() const { return Fl::h(); }
			virtual int screen_left() const { return Fl::x(); }
			virtual int screen_top() const { return Fl::y(); }

			virtual void add_idle(fpCALLBACKpv cb) { Fl::add_idle(cb); }
			virtual void remove_idle(fpCALLBACKpv cb) { Fl::remove_idle(cb); }
			virtual void add_timeout(double t, fpCALLBACKpv cb) { Fl::add_timeout(t, cb); }
			virtual void repeat_timeout(double t, fpCALLBACKpv cb) { Fl::repeat_timeout(t, cb); }
			virtual void remove_timeout(fpCALLBACKpv cb) { Fl::remove_timeout(cb); }

			virtual int event() { return Fl::event(); }
			virtual int event_inside(int x1, int y1, int x2, int y2)
				{ return Fl::event_inside(x1, y1, x2, y2); }
			virtual int event_x() { return Fl::event_x(); }
			virtual int event_y() { return Fl::event_y(); }
			virtual int event_button() { return Fl::event_button(); }
			virtual int event_keystate() { return Fl::event_state(); }
			virtual int event_key() { return Fl::event_key(); }
			virtual int event_key(int k) { return Fl::event_key(k); }

			virtual void flush() { Fl::flush(); }

			virtual void redraw() { Fl::redraw(); }

			virtual void lock() { Fl::lock(); }
			virtual void unlock() { Fl::unlock(); }

			virtual void initHwDblBuff() { Fl::visual(FL_DOUBLE | FL_INDEX); }

			virtual void test() {}
		protected:
			void connect_loggings() const
			{
				Fl::warning = logwarning;
				Fl::error   = logerror;
				Fl::fatal   = logfatal;
			}
		private:
			static void logwarning(const char* sz, ...) { TOOLOGw(sz); }
			static void logerror(const char* sz, ...) { TOOLOGe(sz); }
			static void logfatal(const char* sz, ...) { TOOLOGf(sz); throw FatalBreakdown(); }
		};
	} // gui
} // too

#endif
