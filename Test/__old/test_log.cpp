#include <iostream>
#include <cstdlib>
#include <tchar.h>
#include "Toolib/log.h"
std::string toolog::logger::FILENAME = "debug.log";
toolog::ETraceLevel toolog::logger::TRACELEVEL = toolog::TL_ALL;
#include "Toolib/LOGDEF.h"
//#define TOO_USE_VLD
#include "Toolib/debug.h"

namespace testlog2
{
	void f();
}

namespace
{
	using std::cout;
	using std::endl;

	void log_test()
	{
		cout << "=============== log_test =================" << endl;
		TOOLOGe("test1e");
		toolog::log().putE("test1e_noPPDef", __FILE__);
		testlog2::f();
		TOOLOGw("test1warn");
		TOOLOGi("test1inf");
		TOOLOGa("test1all");
	}
} // namespace
int _tmain(int argc, _TCHAR* argv[])
{
	using std::cout;
	using std::endl;

	int iMainRet = 0;
	try
	{
		log_test();
	}
	catch (const std::exception& e)
	{
		cout << "const std::exception& e" << e.what() << endl;
		iMainRet = 1;
	}
	catch (...)
	{
		cout << "..." << endl;
		iMainRet = 2;
	}

	system("PAUSE");

	TOO_DUMP_MEM_LEAKS;
	return iMainRet;
}

