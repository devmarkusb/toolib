// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef FONT_H_INCL
#define FONT_H_INCL

namespace too
{
	namespace gui
	{
		class Font
		{
		public:
			// taken from FLTK
			enum EType {
				F_HELVETICA	= 0,
				F_HELVETICA_BOLD,
				F_HELVETICA_ITALIC,
				F_HELVETICA_BOLD_ITALIC,
				F_COURIER,
				F_COURIER_BOLD,
				F_COURIER_ITALIC,
				F_COURIER_BOLD_ITALIC,
				F_TIMES,
				F_TIMES_BOLD,
				F_TIMES_ITALIC,
				F_TIMES_BOLD_ITALIC,
				F_SYMBOL,
				F_SCREEN,
				F_SCREEN_BOLD,
				F_ZAPF_DINGBATS
			};

			Font(EType f, int iSize = 0) : m_f(f), m_size(iSize) {}
			Font() : m_f(F_HELVETICA), m_size(0) {}
			// default copy and assignment are ok

			EType getType() const { return m_f; }
			int getSize() const { return m_size; }
		private:
			EType m_f;
			int m_size; // in pixel, 0 denotes default
		};
	} // gui
} // too

#endif