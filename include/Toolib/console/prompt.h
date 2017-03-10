// Markus Borris, 2011-17
// This file is part of Toolib library.

//!
/**
*/
//! \file


#ifndef PROMPT_H_INCL_8jr82ctrz2tn
#define PROMPT_H_INCL_8jr82ctrz2tn

#include "Toolib/ignore_arg.h"
#include "Toolib/PPDEFS.h"
#include <chrono>
#if TOO_OS_WINDOWS
#include <conio.h>
#endif
#include <iostream>
#include <thread>


namespace too
{
inline void prompt()
{
#if TOO_OS_WINDOWS && !TOO_OS_WINDOWS_UWP_APP
    if (_kbhit())
        too::ignore_arg(_getch());
    std::cout << "\nPress any key to continue...";
    while (!_kbhit())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
#else
    std::cout << "\nPress ENTER key to continue...";
    char c{};
    std::cin >> c;
#endif
}
} // too

#endif
