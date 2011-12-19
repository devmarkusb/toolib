// test_gui.cpp : Defines the entry point for the console application.
//
//#include <windows.h>
#include <tchar.h>
#include <iostream>
#include <sstream>
#include <limits>
#include "Toolib/gui.h"

#include "Toolib/log.h"
#include "Toolib/LOGDEF.h"
TOOLOG_SET_LOG_FILENAME("test_gui_wcons.log");
TOOLOG_SET_LOG_TRACELEVEL(TL_ALL);

#define TOO_USE_VLD 1
#include "Toolib/debug.h"

namespace
{
	const toogui::IGUILib& gui = TOOGUI_LIB_CHOICE(GUI_W32CONSOLE)

	toogui::IEngine* guiengine()
	{
		static toogui::Engine e(gui.Engine());
		return e;
	}
	namespace moving_shapes
	{
		toogui::IRectangle* pre;
		int dx=0, dy=0;
	}
	namespace changing_shapes
	{
		toogui::IText* txt;
		toogui::IWindow* win;
	}
	void cbMoveRect(void*)
	{
		using namespace moving_shapes;
		pre->move(dx, dy);
		//guiengine()->repeat_timeout(0.1, cbMoveRect);
	}
	void cbRedrawFPS(void*)
	{
		guiengine()->redraw();
		//guiengine()->repeat_timeout(0.025, cbRedrawFPS);
	}
	void showtempinfo()
	{
		std::ostringstream os;
		os <<
			"x: " << changing_shapes::win->x() << "; " <<
			"y: " << changing_shapes::win->y() << "; " <<
			"w: " << changing_shapes::win->w() << "; " <<
			"h: " << changing_shapes::win->h();
		changing_shapes::txt->set(os.str());
	}
	int ehSteeringRect(toogui::Event::EType e)
	{
		using toogui::Event;
		//using toogui::Key; 
		using toogui::EKeyCode; 
		//Key::EType key;
		EKeyCode key;
		switch (e)
		{
		case Event::E_KEYBOARD:
			//key = static_cast<Key::EType>(guiengine()->event_key());
			key = static_cast<EKeyCode>(guiengine()->event_key());
			switch (key)
			{
			case toogui::KEY_LEFT://Key::K_Left : 
				--moving_shapes::dx; showtempinfo(); break;
			case toogui::KEY_UP://Key::K_Up   : 
				--moving_shapes::dy; changing_shapes::win->fullscreen(); break;
			case toogui::KEY_RIGHT://Key::K_Right: 
				++moving_shapes::dx; break;
			case toogui::KEY_DOWN://Key::K_Down : 
				++moving_shapes::dy; changing_shapes::win->fullscreen_off(); break;
			//default:  break;
			}
			return 1;
		default:
			return 0;
		}
	}
	void ic_pre_test()
	{
		using std::cout;
		using std::endl;
		cout << guiengine()->screen_w() << ", " << guiengine()->screen_h() << endl;

		cout << "Press ENTER to quit...";
#undef max
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}
	void gui_test()
	{
		using namespace toogui;

		int sw = guiengine()->screen_w()-2;
		int sh = guiengine()->screen_h()-5;
		Window mainwin(gui.Window(sw, sh, "Hello World!"));
		mainwin->BgColor(Color::C_DARK_CYAN);
		changing_shapes::win = mainwin;
		Pixel pix(gui.Pixel(Point(10, 10)));
		Line li(gui.Line(Point(12, 10), Point(30, 36)));
		Rect re(gui.Rectangle(Point(40, 10), Point(70, 50)));
		re->FillColor(Color::C_YELLOW);
		moving_shapes::pre = re;
		Text txt(gui.Text(Point(0, 3), "Das hier braucht man nicht lesen, nur ein Test-Text."));
		txt->setFont(Font(Font::F_TIMES_BOLD, 12));
		changing_shapes::txt = txt;
		Circle cir(gui.Circle(Point(10, 30), 4));
		cir->SetColors(Color::C_RED, Color::C_DARK_BLUE);
		mainwin->attach(pix);
		mainwin->attach(li);
		mainwin->attach(re);
		mainwin->attach(txt);
		mainwin->attach(cir);
		guiengine()->add_timeout(0.05, cbRedrawFPS); // 0.025
		guiengine()->add_timeout(0.1, cbMoveRect);
		mainwin->attach_eventhandler(ehSteeringRect);
		mainwin->show();
		guiengine()->main();
	}
	void gui_test2()
	{
	}
}

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
int _tmain(int argc, _TCHAR* argv[])
{
	try
	{
		//ic_pre_test();
		gui_test();
		//gui_test2();

	}
	catch (toogui::IEngine::FatalBreakdown)
	{
		TOOLOGf("toogui::IEngine::FatalBreakdown");
	}
	catch (std::exception& e)
	{
		std::ostringstream os;
		os << "std::exception: " << e.what();
		TOOLOGf(os.str());
	}
	catch (...)
	{
		TOOLOGf("Unknown error! (...)");
	}
	//system("PAUSE");
	//TOO_DUMP_MEM_LEAKS;
	return 0;
}

