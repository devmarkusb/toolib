//! \file

#ifndef PROMPT_HPP_8JR82CTRZ2TN
#define PROMPT_HPP_8JR82CTRZ2TN

#include "../config.hpp"
#include "mb/ul/ul.hpp"
#include <chrono>
#if UL_OS_WINDOWS
#include <conio.h>
#endif
#include <iostream>
#include <thread>

namespace mb::too {
inline void prompt() {
#if UL_OS_WINDOWS && !UL_OS_WINDOWS_UWP_APP
    if (_kbhit())
        ul::ignore_unused(_getch());
    std::cout << "\nPress any key to continue...";
    while (!_kbhit())
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
#else
    std::cout << "\nPress ENTER key to continue...";
    std::getchar();
#endif
}
} // namespace mb::too


#endif
