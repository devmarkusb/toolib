#include "Toolib/consts.h"
#include "Toolib/consts.h"
#include "Toolib/log.h"
#include "Toolib/LOGDEF.h"
TOOLOG_SET_LOG_FILENAME("nogame.log");
TOOLOG_SET_LOG_TRACELEVEL(TL_ALL);

#include "nogame.h"


namespace
{
}

int main()
{
	try
	{
		nog::Nogame game;
		return game.main();
	}
	catch (...)
	{
		return too::C_I_PROG_EXIT_FAILURE;
	}
}
