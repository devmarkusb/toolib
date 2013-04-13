// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef COLOR_H_INCL
#define COLOR_H_INCL

namespace too
{
	namespace gui
	{
		class Color
		{
		public:
			enum EType {
				C_BLACK			= 56,
				C_RED			= 88,
				C_GREEN			= 63,
				C_YELLOW		= 95,
				C_BLUE			= 216,
				C_MAGENTA		= 248,
				C_CYAN			= 223,
				C_DARK_RED		= 72,
				C_DARK_GREEN	= 60,
				C_DARK_YELLOW	= 76,
				C_DARK_BLUE		= 136,
				C_DARK_MAGENTA	= 152,
				C_DARK_CYAN		= 140,
				C_WHITE			= 255
			};
			enum ETransparency {
				T_INVISIBLE		=   0,
				T_VISIBLE		= 255
			};

			Color(EType c, ETransparency t) : m_c(c), m_v(t){}
			Color(EType c) : m_c(c), m_v(T_VISIBLE) {}
			explicit Color(int c) : m_c(static_cast<EType>(c)), m_v(T_VISIBLE) {}
			Color() : m_c(C_BLACK), m_v(T_VISIBLE) {}
			//Color(int r, int g, int b) : m_c(?){}
			// default copy and assignment are ok

			void invisible() { m_v = T_INVISIBLE; }
			void visible() { m_v = T_VISIBLE; }

			int as_int() const { return m_c; }
			EType get() const { return m_c; }
			unsigned char visibility() const { return m_v; }
		private:
			EType m_c;
			unsigned char m_v;
		};
	} // gui
} // too

#endif
