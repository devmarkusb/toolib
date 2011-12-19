// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
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