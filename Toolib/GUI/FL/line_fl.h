// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef LINE_FL_H_INCL
#define LINE_FL_H_INCL

#include "../line.h"

namespace too
{
	namespace gui
	{
		class FLLine : public ILine
		{
		public:
			FLLine(const Point& p1, const Point& p2) : ILine(p1, p2) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif