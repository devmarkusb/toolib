// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef TEXT_H_INCL
#define TEXT_H_INCL

#include "shape.h"
#include "font.h"
#include <string>

namespace too
{
	namespace gui
	{
		class IText : public virtual IShape
		{
		public:
			IText(const Point& p, const std::string& s) : m_s(s) { addPoint(p); }
			// default copy and assignment ok

			void set(const std::string& s) { m_s = s; }
			const std::string& get() const { return m_s; }
			void setFont(const Font& f) { m_f = f; }
			const Font& getFont() const { return m_f; }
		protected:
			virtual void draw_details() const=0;
			virtual void move_details(int dx, int dy);
		private:
			std::string m_s;
			Font m_f;
		};
	}
}

#endif