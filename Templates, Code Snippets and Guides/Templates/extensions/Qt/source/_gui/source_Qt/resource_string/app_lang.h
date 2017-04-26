#ifndef APP_LANG_H_hjgc62478t5hguoitbnxv
#define APP_LANG_H_hjgc62478t5hguoitbnxv
#ifdef UIW_CHOICE_QT

#include "Toolib/class/non_copyable.h"
#include "Toolib/string/languages.h"
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_BEGIN"
#include <QGuiApplication>
#include <QString>
#include <QTranslator>
#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNINGS_EXTERNAL_END"
#include <memory>
#include <vector>


namespace app_lang
{
//! Responsible for language translations of the app.
class AppTranslator : private too::non_copyable
{
public:
    //! Language id type per ISO 639-1.
    using lang_id          = too::str::LangID_iso639_1;
    using lang_id_and_text = std::pair<lang_id, std::string>;
    struct invalid_id : public std::runtime_error
    {
        explicit invalid_id(const std::string& s) : std::runtime_error(s) {}
    };

    static const lang_id defaultLanguageChoice;


    explicit AppTranslator(QGuiApplication& app);

    size_t getSupportedLanguageCount() const { return this->supportedLangs.size(); }
    //! Expects \param idx 0..getSupportedLanguageCount.
    lang_id_and_text getSupportedLang(size_t idx) const;
    //! Throws invalid_id if there is no index found.
    size_t findIdx(const lang_id& id) const;

    void translate(const lang_id& id);

private:
    static const QString langfile_prefix;
    static const QString qtlangfile_prefix;
    static const QString transl_file_ext;

    QGuiApplication& q_app;
    const QString path_to_transl_without_trailing_sep;
    std::unique_ptr<QTranslator> translator;
    std::unique_ptr<QTranslator> qt_translator;

    std::vector<lang_id_and_text> supportedLangs;


    void init();
    //! Doesn't need to be called at program start or when 'en' is active.
    //! It does nothing in these cases, but its purpose is to prepare a call
    //! of selectLanguage when someone was selected before.
    void unselectLanguage();
    void selectLanguage(const lang_id& id);
    void selectLanguage(const QLocale& locale);

    struct InNative
    {
        static void sort(std::vector<lang_id_and_text>& langs);
    };
    struct InEnglish
    {
        static void sort(std::vector<lang_id_and_text>& langs);
    };

    template <class LanguageNamesLanguage>
    // LanguageNamesLanguage expected to be either InNative or InEnglish
    void obtainAllSupportedLangs();
    void addAutoLanguageChoice();
};
} // app_lang

#endif // UIW_CHOICE_QT
#endif
