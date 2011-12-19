// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef EVENT_H_INCL
#define EVENT_H_INCL

namespace too
{
	namespace gui
	{
		class Event
		{
		public:
			// taken from FLTK
			enum EType {
				E_NO_EVENT		= 0,  //!< Nothing happened.
				E_PUSH			= 1,  //!< Mouse button pushed.
				E_RELEASE		= 2,  //!< Mouse button released.
				E_ENTER			= 3,  //!< Mouse pointer entered a control.
				E_LEAVE			= 4,  //!< Mouse pointer left a control.
				E_DRAG			= 5,  //!< Mouse moved while button pressed.
				E_FOCUS			= 6,  //!< Control receives keyboard focus.
				E_UNFOCUS		= 7,  //!< Control loses keyboard focus.
				E_KEYBOARD		= 8,  //!< Key pressed.
				E_KEYUP			= 9,  //!< Key released.
				E_CLOSE			= 10, //!< Window was closed.
				E_MOVE			= 11, //!< Mouse moved.
				E_SHORTCUT		= 12, //!< Shortcut key pressed.
				E_DEACTIVATE	= 13, //!< Control deactivated.
				E_ACTIVATE		= 14, //!< Control activated.
				E_HIDE			= 15, //!< Control hidden.
				E_SHOW			= 16, //!< Control shown.
				E_PASTE			= 17, //!< Control should paste clipboard content.
				E_SELECTIONCLEAR= 18, //!< Control should clear any selections (for clipboard).
				E_MOUSEWHEEL	= 19, //!< Mousewheel (hor. or ver.) turned
				E_DND_ENTER		= 20, //!< Mouse pointer dragging data enters control.
				E_DND_DRAG		= 21, //!< Mouse pointer moved dragging data.
				E_DND_LEAVE		= 22, //!< Mouse pointer dragging data leaves control.
				E_DND_RELEASE	= 23, //!< Dragged data about being dropped.
				//ETYPE_COUNT		= 24 //!< Not an event, but rough maximum number of them.
			};

			Event(EType e) : m_e(e){}
			Event() : m_e(E_NO_EVENT){}
			// default copy and assignment are ok

			int as_int() const { return m_e; }
		private:
			int m_e;
		};

		//! Must return 0 if the event is still unhandled (other handlers could be tried), non-0 else.
		typedef int (*fpEVENTHANDLERe)(Event::EType);
	} // gui
} // too

#endif