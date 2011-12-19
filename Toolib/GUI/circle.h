// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef CIRCLE_H_INCL
#define CIRCLE_H_INCL

#include "shape.h"

namespace too
{
	namespace gui
	{
		class ICircle : public virtual IShape
		{
		public:
			ICircle(const Point& m, float r) : m_rad(r) { addPoint(m); }
			// default copy and assignment ok

			float Radius() const { return m_rad; }
			void Radius(float r) { m_rad = r; }
		protected:
			virtual void draw_details() const=0;
			virtual void move_details(int dx, int dy);
		private:
			float m_rad;
		};
	}
}

#endif