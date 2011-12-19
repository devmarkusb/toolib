// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef PROMPT_H_INCL_8jr82ctrz2tn
#define PROMPT_H_INCL_8jr82ctrz2tn

#include <conio.h>
#include <iostream>

namespace too
{
	inline void prompt()
	{
		if (_kbhit())
			_getch();
		std::cout << std::endl << "Hit any key...";
		while (!_kbhit());
	}
} // too

#endif