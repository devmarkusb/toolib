#include "logging.h"
#include "Toolib/trace.h"
#include "consts.h"
#include "_gui/interface/globals_gui.h"


namespace logging
{
class LogCallback : public el::LogDispatchCallback
{
protected:
    virtual void handle(const el::LogDispatchData* data) override
    {
        std::ostringstream os;
        os << data->logMessage()->message() << "\n";
        too::trace(os);
    }
};

void OnLogFileSizeReached(const char* FileName, std::size_t)
{
    std::string fn(FileName);
    std::string bak_fn(fn + consts::BACKUP_FILEEXT_SUFFIX);
    if (globals::filesys().FileExists(bak_fn))
        globals::filesys().DeleteFile(bak_fn);
    globals::filesys().CopyFile(fn, bak_fn);
}

void Init()
{
    const std::string prog_exe_dir{consts::PROGRAM_EXE_DIR()};
    el::Configurations m_conf{prog_exe_dir + "logging_default.logconf"};
    el::Configurations m_conf_performance{prog_exe_dir + "logging_performance.logconf"};
    const std::string logfile{consts::PROGRAM_LOG_FILEPATHNAMEEXT()};
    m_conf.setGlobally(el::ConfigurationType::Filename, logfile);
    m_conf_performance.setGlobally(el::ConfigurationType::Filename, logfile);
    el::Loggers::reconfigureLogger("default", m_conf);
    el::Loggers::reconfigureLogger("performance", m_conf_performance);
    el::Loggers::addFlag(el::LoggingFlag::StrictLogFileSizeCheck);
    el::Helpers::installPreRollOutCallback(OnLogFileSizeReached);

    el::Helpers::installLogDispatchCallback<LogCallback>("TRACE");
}
}
