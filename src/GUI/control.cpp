// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#include <algorithm>
#include <functional>
#include "Toolib/GUI/control.h"
#include "Toolib/GUI/shape.h"

namespace too
{
	namespace gui
	{
		void IControl::redraw()
		{
			draw();
		}
		void IControl::draw() const
		{
			std::for_each(m_shapes.begin(), m_shapes.end(), std::mem_fun(&IShape::draw));
			std::for_each(m_controls.begin(), m_controls.end(), std::mem_fun(&IControl::draw));
		}
	} // gui
} // too
