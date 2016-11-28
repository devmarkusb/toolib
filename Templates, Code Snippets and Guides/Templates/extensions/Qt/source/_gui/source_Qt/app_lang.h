#ifndef APP_LANG_H_hjgc62478t5hguoitbnxv
#define APP_LANG_H_hjgc62478t5hguoitbnxv

#include "_common/consts.h"
#include "_common/logging.h"
#include "Toolib/assert.h"
#include "Toolib/class/non_copyable.h"
#include "Toolib/std/std_extensions.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"
#include <QGuiApplication>
#include <QLocale>
#include <QString>
#include <QTranslator>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <memory>


namespace app_lang
{
//! Responsible for language translations of the app.
class AppTranslator : private too::non_copyable
{
public:
    AppTranslator(QGuiApplication& app) : q_app{app}
    {
        this->translator = too::make_unique<QTranslator>();
        this->qt_translator = too::make_unique<QTranslator>();
    }

    void init()
    {
        const QString path_to_transl{consts::EXE_TRANSLATIONS_DIR().c_str()};

        // ### app specific translations ###
        //todo check whether we need to install English first, as fall-back
        if (this->translator->load(QLocale(), AppTranslator::langfile_prefix, {}, path_to_transl))
        {
            const bool ok = this->q_app.installTranslator(this->translator.get());
            TOO_ASSERT(ok);
            if (ok)
                LOG(INFO) << "language loaded: " << uiw::implQt::qs2s(QLocale().name());
            else
                LOG(WARNING) << "could not install language: " << uiw::implQt::qs2s(QLocale().name());
        }
        else
            LOG(WARNING) << "could not load language: " << uiw::implQt::qs2s(QLocale().name());

        // ### Qt common translations ###
        //todo check whether we need to install English first, as fall-back
        if (this->qt_translator->load(QLocale(), AppTranslator::qtlangfile_prefix, {}, path_to_transl))
        {
            const bool ok = this->q_app.installTranslator(this->qt_translator.get());
            TOO_ASSERT(ok);
            if (ok)
                LOG(INFO) << "qt language loaded: " << uiw::implQt::qs2s(QLocale().name());
            else
                LOG(WARNING) << "could not install qt language: " << uiw::implQt::qs2s(QLocale().name());
        }
        else
            LOG(WARNING) << "could not load qt language: " << uiw::implQt::qs2s(QLocale().name());
    }

private:
    QGuiApplication& q_app;
    std::unique_ptr<QTranslator> translator;
    std::unique_ptr<QTranslator> qt_translator;
    static const QString langfile_prefix;
    static const QString qtlangfile_prefix;
};
} // app_lang

#endif
