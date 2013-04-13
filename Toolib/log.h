// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** You can define TOOLOG_NO_LOGGING as global compiler setting to overread all (#define-)logging commands
    and initializations (and includes and implementations).
	Usage/Example:
	1) Put the following in your file containing main.
	\code
	#include "log.h"
	#include "LOGDEF.h"
	TOOLOG_SET_LOG_FILENAME("myapp.log");
	TOOLOG_SET_LOG_TRACELEVEL(TL_ALL);
	\endcode
	2) Add src/log/log.cpp to your project.
	3) Put the following in every file, where you want to write logs.
	\code
	#include "log.h"
	#include "LOGDEF.h"
	\endcode
	4) Use TOOLOGe("something to log..."); or TOOLOG<f|w|i|a>("other log"); as you like it.
	5) If you want to use the ("evil") stream macro extension, define "#define TOOLOG_USE_STREAM_MACROS 1".
*/
//! \file

#ifndef LOG_H_INCL_mpv6nu3icnzunzxdtd4n5xtc8
#define LOG_H_INCL_mpv6nu3icnzunzxdtd4n5xtc8

#include <string>
#include <fstream>
#include <sstream>
#include <sys/timeb.h>
#include <ctime>
#include "filesys.h"

namespace too
{
	namespace logging
	{
		enum ETraceLevel {
			TL_NO		= 0, //!< No logging at all.
			TL_FATAL	= 1, //!< Fatal errors.
			TL_ERROR	= 2, //!< Errors.
			TL_WARNING	= 3, //!< Warnings.
			TL_INFO		= 4, //!< Informations.
			TL_ALL		= 5, //!< Even more verbose than info.
			ETRACELEVEL_END = 6 // Just marking the element behind the last one.
		};
		const std::string C_AS_TRACELABEL[ETRACELEVEL_END] = {
			" ",
			" FATAL: ",
			" ERROR: ",
			" WARN: ",
			" INFO: ",
			" "
		};
		//! Roughly 0.5MB max. log file size.
		/** Note that this is half the minimum of the tracked information whereas the maximum is two times this value.
		    This is due to the backup roll mechanism, keeping one FILENAME.bak.bak and one FILENAME.bak.
			500KB are roughly 10.000 lines.*/
		const unsigned int C_UL_MAXFILESIZE = 250000; // 1*10^6 approx. 1MB,(uint supported maximum: 2GB)
		//! This class is for writing logs to a file.
		class logger
		{
			friend logger& log();
		public:
			// Exception class for general log errors.
			//class error_log : virtual public std::exception{}; // Not used. Logging shouldn't throw.

			//! Put an intro log.
			void putIntro(const std::string& sMessage =
				std::string("Starting program... (compile date ") + __DATE__ + " time " + __TIME__ + ")"
				) throw()
			{
				try{put(sMessage, TL_FATAL, "", true);}catch(...){}
			}
			//! Put an outro log.
			void putOutro(const std::string& sMessage = "Ending program...") throw()
			{
				try{put(sMessage, TL_FATAL, "", true);}catch(...){}
			}
			//! Throw a fatal error log.
			void putF(const std::string& sMessage, const std::string& sFILE) throw()
			{
				try{put(sMessage, TL_FATAL, sFILE);}catch(...){}
			}
			std::ostringstream putF_; //todo mutex
			void putF_flush()
			{
				try{put(putF_.str(), TL_FATAL);}catch(...){}
			}
			//! Throw an error log.
			void putE(const std::string& sMessage, const std::string& sFILE) throw()
			{
				try{put(sMessage, TL_ERROR, sFILE);}catch(...){}
			}
			//! Throw a warning log.
			void putW(const std::string& sMessage) throw()
			{
				try{put(sMessage, TL_WARNING);}catch(...){}
			}
			//! Throw an info log.
			void putI(const std::string& sMessage) throw()
			{
				try{put(sMessage, TL_INFO);}catch(...){}
			}
			//! Throw an "all" log.
			void putA(const std::string& sMessage) throw()
			{
				try{put(sMessage, TL_ALL);}catch(...){}
			}

			//! In bytes (10^6 approx. 1MB). \see C_UL_MAXFILESIZE for default value and further information.
			void MaxFileSize(unsigned long s) { m_lMaxFileSize = s; }
			unsigned long MaxFileSize() const { return m_lMaxFileSize; }
		private:
			//! The following data members are declared but not defined.
			/** If the user forgets to initialize them, a linker error occurs. The basic idea was that
			this class can be used even before main() is entered. FILENAME denotes the name of the log file
			to be used. Without path it is assumed to be located in the exe-folder. TRACELEVEL sets the initial
			level of traces that are written at least.*/
			static std::string FILENAME;
			//! \see FILENAME
			static ETraceLevel TRACELEVEL;
			//! Bytes.
			unsigned int m_lMaxFileSize;
			//! Bytes.
			unsigned int m_lSizeCounter;
			//! Set to true, if operations are called that log by themselves.
			friend class SelfLogScope_begin;
			static bool m_bSelfLog;
			class SelfLogScope_begin
			{
			public:
				SelfLogScope_begin(){m_bSelfLog=true;}
				~SelfLogScope_begin(){try{m_bSelfLog=false;}catch(...){}}
			};

			logger() : m_lMaxFileSize(C_UL_MAXFILESIZE), m_lSizeCounter(0) {}
			logger(const logger&);
			logger& operator=(const logger&);

			void put(const std::string& sMessage, ETraceLevel eTraceLevel, const std::string& sFILE = "",
				bool bSuppressLabels = false) throw()
			{
				try{
					if (eTraceLevel > TRACELEVEL || m_bSelfLog)
						return;
					std::string sLine = getTimestamp();
					if (!bSuppressLabels)
					{
						sLine+= C_AS_TRACELABEL[eTraceLevel] + sMessage;
						if (!sFILE.empty())
						{
							std::string sFilename = file::ExtractFilename(sFILE);
							sLine+= std::string(" (") + sFilename + ")";
						}
					}
					else
						sLine+= " " + sMessage;
					put(sLine);
					checkLogFileSize();
				}
				catch(...){}
			}
			void put(const std::string& sMessage) throw()
			{
				try{
					//todo make it thread-safe, mutex
					std::fstream fsLog(FILENAME.c_str(), std::ios_base::app);
					if (fsLog)
					{
						fsLog << std::endl << sMessage;
						m_lSizeCounter+= static_cast<unsigned int>(sMessage.length());
					}
					else
					{
					}
				}
				catch(...){}
			}
			const std::string getTimestamp() const throw()
			{
				try{
					struct _timeb timebuffer;
					_ftime_s(&timebuffer);
					//struct time_t timesec;
					const size_t C_TIMESTRINGLENGTH = 26;
					char timeline[C_TIMESTRINGLENGTH];
					errno_t err = ctime_s(timeline, C_TIMESTRINGLENGTH, &(timebuffer.time));
					if (err)
						return "";
					timeline[C_TIMESTRINGLENGTH - 2] = 0; // removes a \n
					const size_t C_TIMESTRLEN_WITH_MS = C_TIMESTRINGLENGTH + 4;
					char timeline_ms[C_TIMESTRLEN_WITH_MS+1];
					size_t ret = sprintf_s(timeline_ms, C_TIMESTRLEN_WITH_MS, "%.19s.%.3hu %s",
						timeline, timebuffer.millitm, &timeline[20]);
					if (ret == -1)
						return "";
					return timeline_ms;
				}
				catch(...){return "";}
			}
			void checkLogFileSize() throw()
			{
				try{
					static bool bFirstLog = true;
					if (bFirstLog)
					{
						bFirstLog = false;
						int i = file::FileSize(FILENAME.c_str());
						if (i <= 0)
							return; // strange, better do nothing
						m_lSizeCounter+= static_cast<unsigned int>(i);
					}
					if (m_lSizeCounter >= m_lMaxFileSize)
					{
						m_lSizeCounter = 0;
						rollLogBackups();
					}
				}
				catch(...){}
			}
			void rollLogBackups() const throw()
			{
				try{
					// The following file operations log in error case. So we better switch logging off
					// to avoid ugliest neverending loops.
					SelfLogScope_begin sl;
					std::string sbakbak = FILENAME + ".bak.bak";
					file::DelFile(sbakbak.c_str());
					std::string sbak = FILENAME + ".bak";
					file::RenameFile(sbak.c_str(), sbakbak.c_str());
					file::DelFile(sbak.c_str());
					file::RenameFile(FILENAME.c_str(), sbak.c_str());
				}
				catch(...){}
			}
		};

		//! Provides the entry point for the only logger object that exists (statically).
		logger& log();
	} // logging
} // too

namespace toolog = too::logging;

#endif // include guard
