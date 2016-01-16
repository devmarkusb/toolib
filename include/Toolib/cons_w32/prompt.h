// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef PROMPT_H_INCL_8jr82ctrz2tn
#define PROMPT_H_INCL_8jr82ctrz2tn

#include <conio.h>
#include <iostream>
#include <thread>
#include <chrono>
#include "Toolib/argsused.h"


namespace too
{
inline void prompt()
{
    if (_kbhit())
        too::ignore_arg(_getch());
    std::cout << "\nHit any key...";
    while (!_kbhit())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
}
} // too

#endif
