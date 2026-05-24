//! \file

#include "mb/toolib/console/win/catch_exit.h"
#if UL_OS_WINDOWS && UL_OS_WINDOWS_DESKTOP
#include <sstream>
#include <stdexcept>

namespace mb::too {
namespace con {
bool ExitCatcher::m_bRequestedExit = false;

BOOL ExitCatcher::CtrlHandler(DWORD fdwCtrlType) {
    switch (fdwCtrlType) {
        case CTRL_C_EVENT:
            m_bRequestedExit = true;
            return (TRUE);
        case CTRL_CLOSE_EVENT:
            m_bRequestedExit = true;
            return (TRUE);
        case CTRL_BREAK_EVENT:
            m_bRequestedExit = true;
            return FALSE;
        case CTRL_LOGOFF_EVENT:
            m_bRequestedExit = true;
            return FALSE;
        case CTRL_SHUTDOWN_EVENT:
            m_bRequestedExit = true;
            return FALSE;
        default:
            m_bRequestedExit = true;
            return FALSE;
    }
}

ExitCatcher::ExitCatcher() {
    if (!SetConsoleCtrlHandler(reinterpret_cast<PHANDLER_ROUTINE>(CtrlHandler), TRUE)) {
        std::ostringstream os;
        os << "W32ConsExitCatcher: SetConsoleCtrlHandler() error: " << GetLastError();
        throw std::runtime_error(os.str());
    }
}

ExitCatcher& ExitCatcher::getInstance() {
    static ExitCatcher ec;
    return ec;
}
} // namespace con
} // namespace mb::too
#endif // UL_OS_WINDOWS
