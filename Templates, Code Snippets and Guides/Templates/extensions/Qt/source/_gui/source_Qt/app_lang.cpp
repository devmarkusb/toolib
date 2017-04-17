#include "app_lang.h"
#include "_common/app_cfg.h"
#include "_common/logging.h"
#include "_gui/interface/filesys.h"
#include "_gui/interface/progsettings.h"
#include "_gui/interface/resource_string.h"
#include "Toolib/assert.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/std/std_extensions.h"
#include "uiwrap/string/impl_Qt/language_id_convert_Qt.h"
#include "uiwrap/string/impl_Qt/StringConvert_Qt.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QLocale>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <algorithm>


namespace app_lang
{
const QString AppTranslator::langfile_prefix{QStringLiteral("lang_")};
const QString AppTranslator::qtlangfile_prefix{QStringLiteral("qt_")};
const QString AppTranslator::transl_file_ext{QStringLiteral(".qm")};
const AppTranslator::lang_id AppTranslator::defaultLanguageChoice{too::str::auto_systems_language};

AppTranslator::AppTranslator(QGuiApplication& app)
    : q_app{app}
    , path_to_transl_without_trailing_sep{uiw::implQt::s2qs(app::exe_translations_dir())}
    , translator{too::make_unique<QTranslator>()}
    , qt_translator{too::make_unique<QTranslator>()}
{
    init();
}

void AppTranslator::init()
{
    TOO_EXPECT(this->supportedLangs.empty());
    obtainAllSupportedLangs<InEnglish>();
    addAutoLanguageChoice();

    auto id = app::settings().ValueStr(app::setting::key::section_main, app::setting::key::main::language,
        app_lang::AppTranslator::defaultLanguageChoice);
    selectLanguage(id);
}

AppTranslator::lang_id_and_text AppTranslator::getSupportedLang(size_t idx) const
{
    TOO_EXPECT(idx < this->supportedLangs.size());
    return this->supportedLangs[idx];
}

size_t AppTranslator::findIdx(const lang_id& id) const
{
    const auto it = std::find_if(std::begin(this->supportedLangs), std::end(this->supportedLangs),
        [&id](const lang_id_and_text& item)
        {
            return item.first == id;
        });
    if (it == std::end(this->supportedLangs))
        throw invalid_id{id};
    return std::distance(std::begin(this->supportedLangs), it);
}

void AppTranslator::translate(const lang_id& id)
{
    unselectLanguage();
    selectLanguage(id);
}

void AppTranslator::unselectLanguage()
{
    if (!this->q_app.removeTranslator(this->translator.get()))
        LOG(TRACE) << "could not remove language (no error, if it was English)";
    if (!this->q_app.removeTranslator(this->qt_translator.get()))
        LOG(TRACE) << "could not remove language (no error, if it was English)";
}

void AppTranslator::selectLanguage(const lang_id& id)
{
    auto locale = QLocale{};
    if (id != too::str::auto_systems_language)
    {
        try
        {
            locale = QLocale{uiw::implQt::fromLanguageID(id)};
        }
        catch (const too::not_implemented& e)
        {
            LOG(ERROR) << "could not select language " << id << ", details: " << e.what();
            locale = QLocale{};
        }
    }
    selectLanguage(locale);
}

void AppTranslator::selectLanguage(const QLocale& locale)
{
    // sepcial handling English: no need to install translators
    if (locale.language() == QLocale::English)
        return;

    // ### app specific translations ###
    if (this->translator->load(locale, AppTranslator::langfile_prefix, {}, this->path_to_transl_without_trailing_sep))
    {
        const bool ok = this->q_app.installTranslator(this->translator.get());
        if (ok)
            LOG(INFO) << "language loaded: " << uiw::implQt::qs2s(locale.name());
        else
        {
            LOG(WARNING) << "could not install language: " << uiw::implQt::qs2s(locale.name());
        }
        TOO_ASSERT(ok);
    }
    else
    {
        LOG(WARNING) << "could not load language: " << uiw::implQt::qs2s(locale.name());
    }

    // ### Qt common translations ###
    if (this->qt_translator->load(
            locale, AppTranslator::qtlangfile_prefix, {}, this->path_to_transl_without_trailing_sep))
    {
        const bool ok = this->q_app.installTranslator(this->qt_translator.get());
        if (ok)
            LOG(INFO) << "qt language loaded: " << uiw::implQt::qs2s(locale.name());
        else
        {
            LOG(WARNING) << "could not install qt language: " << uiw::implQt::qs2s(locale.name());
        }
        TOO_ASSERT(ok);
    }
    else
    {
        LOG(WARNING) << "could not load qt language: " << uiw::implQt::qs2s(locale.name());
    }
}

void AppTranslator::InNative::sort(std::vector<lang_id_and_text>&) { TOO_NOOP; }

void AppTranslator::InEnglish::sort(std::vector<lang_id_and_text>& langs)
{
    std::sort(std::begin(langs), std::end(langs), [](const lang_id_and_text& lhs, const lang_id_and_text& rhs)
        {
            return lhs.second < rhs.second;
        });
}

template <class LanguageNamesLanguage>
void AppTranslator::obtainAllSupportedLangs()
{
    TOO_EXPECT(this->supportedLangs.empty());
    auto ids = too::str::getAllTheoreticallyPossibleLangIDs();
    // remove English from the search list but add it to the result list anyway,
    // since it is the special case of the non-translated development language
    auto en_iterator = std::find_if(std::begin(ids), std::end(ids), [](const too::str::LangID_iso639_1& id)
        {
            return id == "en";
        });
    ids.erase(en_iterator);
    this->supportedLangs.push_back(std::make_pair("en", too::str::getEnglishLanguageName("en")));
    for (const auto& id : ids)
    {
        const QString id_q{uiw::implQt::s2qs(id)};
        const QString transl_file{this->path_to_transl_without_trailing_sep + QStringLiteral("/") +
            AppTranslator::langfile_prefix + id_q + AppTranslator::transl_file_ext};
        const std::string transl_file_{uiw::implQt::qs2s(transl_file)};
        if (!os::filesys().FileExists(transl_file_))
            continue;
        try
        {
            const QLocale::Language lang{uiw::implQt::fromLanguageID(id)};
            const auto locale = QLocale{lang};
            if (!this->translator->load(
                    locale, AppTranslator::qtlangfile_prefix, {}, this->path_to_transl_without_trailing_sep))
                continue;
            this->supportedLangs.push_back(std::make_pair(id, too::str::getEnglishLanguageName(id)));
        }
        catch (const too::not_implemented& e)
        {
            LOG(ERROR) << "couldn't accept language " << id << ", details: " << e.what();
            continue;
        }
    }
    LanguageNamesLanguage::sort(this->supportedLangs);
}

void AppTranslator::addAutoLanguageChoice()
{
    this->supportedLangs.insert(std::begin(this->supportedLangs),
        std::make_pair(too::str::auto_systems_language, res::getResourceString(res::ID::STR_LANGUAGE_AUTO_SYSTEM)));
}
} // app_lang
