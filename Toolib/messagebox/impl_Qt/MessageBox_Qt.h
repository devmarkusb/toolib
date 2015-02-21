// Markus Borris, 2014
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16
#define MESSAGEBOX_QT_H_sidxgezwgfw76rn3x16

#include <QMessageBox>
#include "../imessagebox.h"
#include "../../class/non_copyable.h"
#include "../../../Toolib_QtDEF.h"


namespace too
{
namespace implQt
{

class TOOLIBSHARED_EXPORT CMessageBox_Qt : public too::gui::IMessageBox, private too::non_copyable
{
public:
    virtual ~CMessageBox_Qt() = default;

    virtual void AddButton(EButton b);
    virtual void SetDefaultButton(EButton b);
    virtual void SetEscapeButton(EButton b);
    virtual void SetCaption(const too::string& s);
    virtual void SetText(const too::string& s);
    virtual void SetDetailedText(const too::string& s);
    virtual void SetIcon(EIcon i);

    virtual bool RunModal();

    virtual EButton GetRunResult();

private:
    QMessageBox                 m_MsgBox;
    QMessageBox::StandardButton m_RetVal{QMessageBox::NoButton};

    QMessageBox::StandardButton EButton2StandardButton(EButton b);
    EButton StandardButton2EButton(QMessageBox::StandardButton qb);
};

}
}

#endif
