// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef IMESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3
#define IMESSAGEBOX_H_ydugfbxuzgnfv8eqgwgw23d3

#include <memory>
#include "../types.h"


namespace too
{
namespace gui
{
class IMessageBox
{
public:
    virtual ~IMessageBox() = default;

    enum class EButton
    {
        OK,
        CANCEL,
        YES,
        YESTOALL,
        NO,
        NOTOALL,
        CLOSE,
        IGNORE,
        RETRY,
        OPEN,
        SAVE,
        SAVEALL,
        RESET,
        DISCARD,
        UNDO = DISCARD,
        APPLY,
        HELP,
        RESTOREDEFAULTS,
        ABORT,
    };

    enum class EIcon
    {
        NONE,
        INFORMATION,
        QUESTION,
        WARNING,
        CRITICAL,
    };

    static std::unique_ptr<IMessageBox> make();

    virtual void AddButton(EButton b) = 0;
    virtual void SetDefaultButton(EButton b) = 0;
    virtual void SetEscapeButton(EButton b) = 0;
    virtual void SetCaption(const too::string& s) = 0;
    virtual void SetText(const too::string& s) = 0;
    virtual void SetDetailedText(const too::string& s) = 0;
    virtual void SetIcon(EIcon i) = 0;

    virtual bool RunModal() = 0;

    virtual EButton GetRunResult() = 0;
};
}
}

#endif
