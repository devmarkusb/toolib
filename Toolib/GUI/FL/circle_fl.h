// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef CIRCLE_FL_H_INCL
#define CIRCLE_FL_H_INCL

#include "../circle.h"

namespace too
{
	namespace gui
	{
		class FLCircle : public ICircle
		{
		public:
			FLCircle(const Point& m, float r) : ICircle(m, r) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
