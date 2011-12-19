// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#ifndef TOOGUI_DONT_LINK_FLTK
#include "Toolib/GUI/FL/gui_FL.h"
#endif
#ifndef TOOGUI_DONT_LINK_WCONS
#include "Toolib/GUI/WCONS/gui_WCONS.h"
#endif

namespace too
{
	namespace gui
	{
		const IGUILib& IGUILib::create(EGUI_Choices gui)
		{
			if (gui == GUI_W32CONSOLE)
			{
				static const
#ifndef TOOGUI_DONT_LINK_WCONS
					WCONS_GUILib
#else
					FL_GUILib
#endif
					guichoice;
				static const IGUILib& usegui = guichoice;
				return usegui;
			}
			else // GUI_FLTK
			{
				static const
#ifndef TOOGUI_DONT_LINK_FLTK
					FL_GUILib
#else
					WCONS_GUILib
#endif
					guichoice;
				static const IGUILib& usegui = guichoice;
				return usegui;
			}
		}
	}
}
