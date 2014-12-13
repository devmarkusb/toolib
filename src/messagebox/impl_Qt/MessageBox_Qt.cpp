#include "../../../Toolib/messagebox/impl_Qt/MessageBox_Qt.h"
#include "../../../Toolib/string/impl_Qt/StringConvert_Qt.h"


namespace too
{
namespace implQt
{

QMessageBox::StandardButton CMessageBox_Qt::EButton2StandardButton(EButton b)
{
    QMessageBox::StandardButton qb = QMessageBox::Ok;
    switch (b)
    {
    case EButton::CANCEL: qb = QMessageBox::Cancel; break;
    case EButton::YES: qb = QMessageBox::Yes; break;
    case EButton::YESTOALL: qb = QMessageBox::YesToAll; break;
    case EButton::NO: qb = QMessageBox::No; break;
    case EButton::NOTOALL: qb = QMessageBox::NoToAll; break;
    case EButton::CLOSE: qb = QMessageBox::Close; break;
    case EButton::IGNORE: qb = QMessageBox::Ignore; break;
    case EButton::RETRY: qb = QMessageBox::Retry; break;
    case EButton::OPEN: qb = QMessageBox::Open; break;
    case EButton::SAVE: qb = QMessageBox::Save; break;
    case EButton::SAVEALL: qb = QMessageBox::SaveAll; break;
    case EButton::RESET: qb = QMessageBox::Reset; break;
    case EButton::DISCARD: qb = QMessageBox::Discard; break;
    case EButton::APPLY: qb = QMessageBox::Apply; break;
    case EButton::HELP: qb = QMessageBox::Help; break;
    case EButton::RESTOREDEFAULTS: qb = QMessageBox::RestoreDefaults; break;
    case EButton::ABORT: qb = QMessageBox::Abort; break;
    case EButton::OK:
    default: qb = QMessageBox::Ok;
    }
    return qb;
}

CMessageBox_Qt::EButton CMessageBox_Qt::StandardButton2EButton(QMessageBox::StandardButton qb)
{
    EButton b = EButton::OK;
    switch (qb)
    {
    case QMessageBox::Cancel: b = EButton::CANCEL; break;
    case QMessageBox::Yes: b = EButton::YES; break;
    case QMessageBox::YesToAll: b = EButton::YESTOALL; break;
    case QMessageBox::No: b = EButton::NO; break;
    case QMessageBox::NoToAll: b = EButton::NOTOALL; break;
    case QMessageBox::Close: b = EButton::CLOSE; break;
    case QMessageBox::Ignore: b = EButton::IGNORE; break;
    case QMessageBox::Retry: b = EButton::RETRY; break;
    case QMessageBox::Open: b = EButton::OPEN; break;
    case QMessageBox::Save: b = EButton::SAVE; break;
    case QMessageBox::SaveAll: b = EButton::SAVEALL; break;
    case QMessageBox::Reset: b = EButton::RESET; break;
    case QMessageBox::Discard: b = EButton::DISCARD; break;
    case QMessageBox::Apply: b = EButton::APPLY; break;
    case QMessageBox::Help: b = EButton::HELP; break;
    case QMessageBox::RestoreDefaults: b = EButton::RESTOREDEFAULTS; break;
    case QMessageBox::Abort: b = EButton::ABORT; break;
    case QMessageBox::Ok:
    default: b = EButton::OK;
    }
    return b;
}

void CMessageBox_Qt::AddButton(too::gui::IMessageBox::EButton b)
{
    m_MsgBox.addButton(EButton2StandardButton(b));
}

void CMessageBox_Qt::SetDefaultButton(too::gui::IMessageBox::EButton b)
{
    m_MsgBox.setDefaultButton(EButton2StandardButton(b));
}

void CMessageBox_Qt::SetEscapeButton(too::gui::IMessageBox::EButton b)
{
    m_MsgBox.setEscapeButton(EButton2StandardButton(b));
}

void CMessageBox_Qt::SetCaption(const too::string& s)
{
    m_MsgBox.setText(toos2qs(s));
}

void CMessageBox_Qt::SetText(const too::string& s)
{
    m_MsgBox.setInformativeText(toos2qs(s));
}

void CMessageBox_Qt::SetDetailedText(const too::string& s)
{
    m_MsgBox.setDetailedText(toos2qs(s));
}

void CMessageBox_Qt::SetIcon(too::gui::IMessageBox::EIcon i)
{
    QMessageBox::Icon qi = QMessageBox::NoIcon;
    switch(i)
    {
    case EIcon::INFORMATION: qi = QMessageBox::Information; break;
    case EIcon::QUESTION: qi = QMessageBox::Question; break;
    case EIcon::WARNING: qi = QMessageBox::Warning; break;
    case EIcon::CRITICAL: qi = QMessageBox::Critical; break;
    case EIcon::NONE:
    default: qi = QMessageBox::NoIcon;
    }
    m_MsgBox.setIcon(qi);
}

bool CMessageBox_Qt::RunModal()
{
    m_RetVal = static_cast<QMessageBox::StandardButton>(m_MsgBox.exec());
    return true;
}

too::gui::IMessageBox::EButton CMessageBox_Qt::GetRunResult()
{
    return StandardButton2EButton(m_RetVal);
}

}
}
