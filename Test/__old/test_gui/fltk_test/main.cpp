//#include <windows.h>
#include <FL/Fl.h>
#include <FL/Fl_Window.h>

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
int main()
{
	Fl_Window window(200, 200, "Window title");
	window.show();
	return Fl::run();
}