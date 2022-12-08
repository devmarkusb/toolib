// 2011-17

//!
/**
 */
//! \file


#ifndef PROMPT_H_8jr82ctrz2tn
#define PROMPT_H_8jr82ctrz2tn

#include "ul/ul.h"
#include <chrono>
#if UL_OS_WINDOWS
#include <conio.h>
#endif
#include <iostream>
#include <thread>

#include "ul/macros.h"


namespace mb::too
{
inline void prompt()
{
#if UL_OS_WINDOWS && !UL_OS_WINDOWS_UWP_APP
    if (_kbhit())
        ul::ignore_arg(_getch());
    std::cout << "\nPress any key to continue...";
    while (!_kbhit())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
#else
    std::cout << "\nPress ENTER key to continue...";
    char c{};
    std::cin >> c;
#endif
}
} // namespace mb::too

#include "ul/macros_end.h"

#endif
