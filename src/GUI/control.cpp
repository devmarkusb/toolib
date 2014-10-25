// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

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
