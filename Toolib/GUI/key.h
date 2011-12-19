// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef KEY_H_INCL
#define KEY_H_INCL

namespace too
{
	namespace gui
	{
		class Key
		{
		public:
			// Definitions taken from FLTK.
			//! Use ascii letters for all other keys.
			enum EType {
				K_NoKey			= 0x0000,
				K_BackSpace		= 0xff08,
				K_Tab			= 0xff09,
				K_Enter			= 0xff0d,
				K_Pause			= 0xff13,
				K_Scroll_Lock	= 0xff14,
				K_Escape		= 0xff1b,
				K_Home			= 0xff50,
				K_Left			= 0xff51,
				K_Up			= 0xff52,
				K_Right			= 0xff53,
				K_Down			= 0xff54,
				K_Page_Up		= 0xff55,
				K_Page_Down		= 0xff56,
				K_End			= 0xff57,
				K_Print			= 0xff61,
				K_Insert		= 0xff63,
				K_Menu			= 0xff67, // the "menu/apps" key on XFree86
				K_Help			= 0xff68, // the 'help' key on Mac keyboards
				K_Num_Lock		= 0xff7f,
				K_KP			= 0xff80, // use K_KP+'x' for 'x' on numeric keypad
				K_KP_Enter		= 0xff8d, // same as K_KP+'\r'
				K_KP_Last		= 0xffbd, // use to range-check keypad
				K_F				= 0xffbd, // use K_F+n for function key n
				K_F_Last		= 0xffe0, // use to range-check function keys
				K_Shift_L		= 0xffe1,
				K_Shift_R		= 0xffe2,
				K_Control_L		= 0xffe3,
				K_Control_R		= 0xffe4,
				K_Caps_Lock		= 0xffe5,
				K_Meta_L		= 0xffe7, // the left MSWindows key on XFree86
				K_Meta_R		= 0xffe8, // the right MSWindows key on XFree86
				K_Alt_L			= 0xffe9,
				K_Alt_R			= 0xffea,
				K_Delete		= 0xffff
			};
			enum EState {
				KS_SHIFT		= 0x00010000,
				KS_CAPS_LOCK	= 0x00020000,
				KS_CTRL			= 0x00040000,
				KS_ALT			= 0x00080000,
				KS_NUM_LOCK		= 0x00100000, // most X servers do this?
				KS_META			= 0x00400000, // correct for XFree86
				KS_SCROLL_LOCK	= 0x00800000, // correct for XFree86
#ifdef __APPLE__
				KS_COMMAND		= KS_META,
#else
				KS_COMMAND		= KS_CTRL
#endif
			};

			Key(EType k) : m_k(k){}
			explicit Key(int k) : m_k(static_cast<EType>(k)){}
			Key() : m_k(K_NoKey){}
			// default copy and assignment are ok

			EType get() const { return m_k; }
		private:
			EType m_k;
		};

		class Mouse
		{
		public:
			enum EMouseButton {
				MB_LEFT_MOUSE	= 1,
				MB_MIDDLE_MOUSE	= 2,
				MB_RIGHT_MOUSE	= 3
			};
		};
	} // gui
} // too

#endif