// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef GUI_H_INCL_cpewoifweofewiucr89243
#define GUI_H_INCL_cpewoifweofewiucr89243

#include <string>
#include "scope/autoptr.h"

namespace too
{
	namespace gui
	{
		struct Point;
		class IEngine;
		class IWindow;
		class IPixel;
		class ILine;
		class IRectangle;
		class IText;
		class ICircle;
		class IControl;

		enum EGUI_Choices {
			GUI_FLTK,
			GUI_W32CONSOLE
		};

		class IGUILib
		{
		public:
			static const IGUILib& create(EGUI_Choices gui);
			virtual ~IGUILib(){}
			// default copy and assignment ok

			virtual IEngine* Engine() const = 0;
			virtual IWindow* Window(int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0) const = 0;
			virtual IWindow* Window(const Point& left_top, int iWidth, int iHeight, const std::string& sTitle, IControl* parent = 0) const = 0;
			virtual IPixel* Pixel(const Point& p) const = 0;
			virtual ILine* Line(const Point& p1, const Point& p2) const = 0;
			virtual IRectangle* Rectangle(const Point& p1, const Point& p2) const = 0;
			virtual IRectangle* Rectangle(const Point& p1, int iWidth, int iHeight) const = 0;
			virtual IText* Text(const Point& p, const std::string& s) const = 0;
			virtual ICircle* Circle(const Point& m, float r) const = 0;
		};

		typedef autoptr<IEngine> Engine;
		typedef autoptr<IWindow> Window;
		typedef autoptr<IPixel> Pixel;
		typedef autoptr<ILine> Line;
		typedef autoptr<IRectangle> Rect;
		typedef autoptr<IText> Text;
		typedef autoptr<ICircle> Circle;

	} // gui
} // too

namespace toogui = too::gui;

#ifndef TOOGUI_INCLUDE_NOTHING
#include "GUI/point.h"
#include "GUI/color.h"
#include "GUI/font.h"
#include "GUI/event.h"
#include "GUI/key.h"
#include "GUI/keycodes.h"
#include "GUI/gui_types.h"
#include "GUI/gui_engine.h"
#include "GUI/window.h"
#include "GUI/pixel.h"
#include "GUI/line.h"
#include "GUI/rectangle.h"
#include "GUI/text.h"
#include "GUI/circle.h"
#endif

//! Use macro like \code const toogui::IGUILib& gui = TOOGUI_LIB_CHOICE(GUI_FLTK) \endcode
/** \param lib out of EGUI_Choices*/
#define TOOGUI_LIB_CHOICE(lib) toogui::IGUILib::create(toogui::lib);

#endif
