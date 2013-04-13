// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


//#ifndef WINDOW_HEADER_INCL
//#define WINDOW_HEADER_INCL

#include "control.h"

namespace too
{
	namespace gui
	{
		class IWindow : public IControl
		{
		public:
			explicit IWindow(std::string sLabel = "") : IControl(sLabel) {}
			explicit IWindow(IControl* parent, std::string sLabel = "") : IControl(parent, sLabel) {}
			// default copy and assignment forbidden for base

			virtual ~IWindow(){}

			virtual void fullscreen()=0;
			virtual void fullscreen_off()=0;

			virtual void redraw() { IControl::redraw(); }
		};
	} // gui
} // too

//#endif
