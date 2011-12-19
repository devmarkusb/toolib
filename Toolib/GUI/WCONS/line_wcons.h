// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef LINE_WCONS_H_INCL
#define LINE_WCONS_H_INCL

#include "../line.h"

namespace too
{
	namespace gui
	{
		class WCONSLine : public ILine
		{
		public:
			WCONSLine(const Point& p1, const Point& p2) : ILine(p1, p2) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif