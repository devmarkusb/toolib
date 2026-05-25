//! \file

#ifndef CATCH_EXIT_HPP_34TRCX83943G
#define CATCH_EXIT_HPP_34TRCX83943G

#include "mb/toolib/config.hpp"
#include "mb/ul/buildenv/macros.hpp"
#if UL_OS_WINDOWS && UL_OS_WINDOWS_DESKTOP
#include <windows.h>

namespace mb::too {
namespace con {
class ExitCatcher {
public:
    //! Use global reference W32ConsExitCatcher instead.
    static ExitCatcher& getInstance();

    bool RequestedExit() {
        if (m_bRequestedExit) {
            m_bRequestedExit = false;
            return true;
        } else {
            return false;
        }
    }

private:
    static bool m_bRequestedExit;

    static BOOL CtrlHandler(DWORD fdwCtrlType);

    ExitCatcher();
};
} // namespace con
} // namespace mb::too
#endif // UL_OS_WINDOWS

#endif
