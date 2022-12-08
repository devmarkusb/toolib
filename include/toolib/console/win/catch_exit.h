// 2011

//!
/**
 */
//! \file


#ifndef CATCH_EXIT_H_34trcx83943g
#define CATCH_EXIT_H_34trcx83943g

#include "ul/macros.h"
#if UL_OS_WINDOWS && UL_OS_WINDOWS_DESKTOP
#include "../../../toolibDEF.h"
#include <windows.h>

namespace mb::too
{
namespace con
{
class TOOLIBSHARED_EXPORT ExitCatcher
{
public:
    //! Use global reference W32ConsExitCatcher instead.
    static ExitCatcher& getInstance();

    bool RequestedExit()
    {
        if (m_bRequestedExit)
        {
            m_bRequestedExit = false;
            return true;
        }
        else
        {
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

#include "ul/macros_end.h"

#endif
