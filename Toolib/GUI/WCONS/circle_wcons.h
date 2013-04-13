// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef CIRCLE_WCONS_H_INCL
#define CIRCLE_WCONS_H_INCL

#include "../circle.h"

namespace too
{
	namespace gui
	{
		class WCONSCircle : public ICircle
		{
		public:
			WCONSCircle(const Point& m, float r) : ICircle(m, r) {}
			// default copy and assignment ok

		protected:
			virtual void draw_details() const;
		};
	}
}

#endif
