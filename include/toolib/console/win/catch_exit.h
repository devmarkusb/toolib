// Markus Borris, 2011
// This file is part of toolib library.

//!
/**
*/
//! \file


#ifndef CATCH_EXIT_H_INCL_34trcx83943g
#define CATCH_EXIT_H_INCL_34trcx83943g

#include "toolib/PPDEFS.h"
#if TOO_OS_WINDOWS && TOO_OS_WINDOWS_DESKTOP
#include "../../../toolibDEF.h"
#include <windows.h>

namespace too
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
}
}
#endif // TOO_OS_WINDOWS

#endif
