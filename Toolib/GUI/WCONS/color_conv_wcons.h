// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file


#ifndef COLOR_CONV_WCONS_H_INCL
#define COLOR_CONV_WCONS_H_INCL

#include <boost/bimap.hpp>
#include "../color.h"
#include "../../cons_w32/cons_w32.h"

namespace too
{
	namespace gui
	{
		namespace WCONS
		{
			using namespace boost::bimaps;
			class color_conv_ic
			{
			private:
				color_conv_ic();

				typedef bimap<Color::EType, con::BgColor> BimBgColor;
				typedef BimBgColor::value_type pos;
				static BimBgColor bimBg;

				typedef bimap<Color::EType, con::TextColor> BimColor;
				typedef BimColor::value_type posCol;
				static BimColor bimCol;

				static bool m_bInitialized;
			public:
				static void init()
				{
					if (m_bInitialized)
						return;
					using namespace con;
					bimBg.insert(pos(Color::C_BLACK, BG_BLACK));
					bimBg.insert(pos(Color::C_RED, BG_RED));
					bimBg.insert(pos(Color::C_GREEN, BG_GREEN));
					bimBg.insert(pos(Color::C_YELLOW, BG_YELLOW));
					bimBg.insert(pos(Color::C_BLUE, BG_BLUE));
					bimBg.insert(pos(Color::C_MAGENTA, BG_PINK));
					bimBg.insert(pos(Color::C_CYAN, BG_LIGHTBLUE));
					bimBg.insert(pos(Color::C_DARK_RED, BG_DARKRED));
					bimBg.insert(pos(Color::C_DARK_GREEN, BG_DARKGREEN));
					bimBg.insert(pos(Color::C_DARK_YELLOW, BG_OCHER));
					bimBg.insert(pos(Color::C_DARK_BLUE, BG_DARKBLUE));
					bimBg.insert(pos(Color::C_DARK_MAGENTA, BG_VIOLET));
					bimBg.insert(pos(Color::C_DARK_CYAN, BG_TURQUOISE));
					bimBg.insert(pos(Color::C_WHITE, BG_WHITE));

					bimCol.insert(posCol(Color::C_BLACK, FG_BLACK));
					bimCol.insert(posCol(Color::C_RED, FG_RED));
					bimCol.insert(posCol(Color::C_GREEN, FG_GREEN));
					bimCol.insert(posCol(Color::C_YELLOW, FG_YELLOW));
					bimCol.insert(posCol(Color::C_BLUE, FG_BLUE));
					bimCol.insert(posCol(Color::C_MAGENTA, FG_PINK));
					bimCol.insert(posCol(Color::C_CYAN, FG_LIGHTBLUE));
					bimCol.insert(posCol(Color::C_DARK_RED, FG_DARKRED));
					bimCol.insert(posCol(Color::C_DARK_GREEN, FG_DARKGREEN));
					bimCol.insert(posCol(Color::C_DARK_YELLOW, FG_OCHER));
					bimCol.insert(posCol(Color::C_DARK_BLUE, FG_DARKBLUE));
					bimCol.insert(posCol(Color::C_DARK_MAGENTA, FG_VIOLET));
					bimCol.insert(posCol(Color::C_DARK_CYAN, FG_TURQUOISE));
					bimCol.insert(posCol(Color::C_WHITE, FG_WHITE));

					m_bInitialized = true;
				}
				static Color::EType ic2toogui_bg(con::BgColor c)
				{
					if (m_bInitialized)
					{
						BimBgColor::right_const_iterator right_iter = bimBg.right.find(c);
						return right_iter->second;
					}
					else
						return Color::C_BLACK;
				}
				static con::BgColor toogui2ic_bg(Color::EType c)
				{
					if (m_bInitialized)
					{
						BimBgColor::left_const_iterator left_iter = bimBg.left.find(c);
						return left_iter->second;
					}
					else
						return con::BG_BLACK;
				}
				static Color::EType ic2toogui(con::TextColor c)
				{
					if (m_bInitialized)
					{
						BimColor::right_const_iterator right_iter = bimCol.right.find(c);
						return right_iter->second;
					}
					else
						return Color::C_WHITE;
				}
				static con::TextColor toogui2ic(Color::EType c)
				{
					if (m_bInitialized)
					{
						BimColor::left_const_iterator left_iter = bimCol.left.find(c);
						return left_iter->second;
					}
					else
						return con::FG_WHITE;
				}
			}; // class
		} // WCONS
	} // gui
} // too

#endif
