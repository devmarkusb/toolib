// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
//#ifndef GUI_ENGINE_HEADER_INCL
//#define GUI_ENGINE_HEADER_INCL

#include <exception>
#include "gui_types.h"

namespace too
{
	namespace gui
	{
		class IEngine
		{
		public:
			//! Exception.
			class FatalBreakdown : public virtual std::exception{};

			virtual ~IEngine(){}
			// default copy and assignment ok

			virtual int main() = 0;
			virtual int wait() = 0;
			
			virtual int screen_w() const =0;
			virtual int screen_h() const =0;
			virtual int screen_left() const =0; //!< Usually 0.
			virtual int screen_top() const =0; //!< Usually 0.

			virtual void add_idle(fpCALLBACKpv) =0;
			virtual void remove_idle(fpCALLBACKpv) =0;
			/** \see repeat_timeout*/
			virtual void add_timeout(double t, fpCALLBACKpv) =0; //!< t in seconds.
			/** Example: \code
			void callback(void*) {
			 // do sth. ...
			 engine->repeat_timeout(1.0, callback);
			}
			int main() {
			 engine->add_timeout(1.0, callback);
			 return engine->run();
			}\endcode*/
			virtual void repeat_timeout(double t, fpCALLBACKpv) =0; //!< t in seconds.
			virtual void remove_timeout(fpCALLBACKpv cb) =0;

			virtual int event() =0;
			virtual int event_inside(int,int,int,int) =0;
			virtual int event_x() =0;
			virtual int event_y() =0;
			virtual int event_button() =0;
			virtual int event_keystate() =0;
			virtual int event_key() =0;
			virtual int event_key(int) =0;

			virtual void flush() =0;

			virtual void redraw() =0;

			virtual void lock() =0;
			virtual void unlock() =0;

			virtual void initHwDblBuff() =0;

			virtual void test() =0;
		};
	} // gui
} // too

//#endif