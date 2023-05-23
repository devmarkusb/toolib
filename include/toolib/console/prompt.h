//! \file


#ifndef PROMPT_H_8JR82CTRZ2TN
#define PROMPT_H_8JR82CTRZ2TN

#include "../config.h"
#include "ul/ul.h"
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

TOO_HEADER_END

#endif
