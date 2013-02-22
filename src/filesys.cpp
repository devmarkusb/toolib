// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#include <fstream>
#include <sstream>
#include <cstdio>
#include "../Toolib/filesys.h"
#include "../Toolib/PPDEFS.h"
#ifdef TOO_WINDOWS
#include <windows.h>
#endif
#include "../Toolib/log.h"
#include "../Toolib/LOGDEF.h"

namespace too
{
	namespace file
	{
		//! Hopefully not larger than int.
		int FileSize(const char* name)
		{
			std::ifstream f;
			f.open(name, std::ios_base::binary | std::ios_base::in);
			if (!f.good() || f.eof() || !f.is_open()) { return 0; }
			f.seekg(0, std::ios_base::beg);
			std::ifstream::pos_type begin_pos = f.tellg();
			f.seekg(0, std::ios_base::end);
			return static_cast<int>(f.tellg() - begin_pos);
		}
		bool DelFile(const char* name)
		{
#ifdef TOO_WINDOWS
			if (!::DeleteFileA(name))
			{
				std::ostringstream os;
				os << "Win: DeleteFile(" << name << ") error: " << GetLastError();
				TOOLOGe(os.str());
				return false;
			}
#else
			if (remove(name))
			{
				std::ostringstream os;
				os << "C: remove(" << name << ") errno: " << errno;
				TOOLOGe(os.str());
				return false;
			}
#endif // or Linux: unlink() ?
			return true;
		}
		bool RenameFile(const char* oldname, const char* newname)
		{
			if (rename(oldname, newname))
			{
				std::ostringstream os;
				os << "C: rename(" << oldname << ", " << newname << ") errno: " << errno;
				TOOLOGe(os.str());
				return false;
			}
			return true;
		}
		std::string ExtractDirectory(const std::string& filenamepath)
		{
#ifdef TOO_WINDOWS
			return filenamepath.substr(0, filenamepath.find_last_of('\\') + 1);
#else
			return filenamepath.substr(0, filenamepath.find_last_of('/') + 1);
#endif
		}
		std::string ExtractFilename(const std::string& filenamepath)
		{
#ifdef TOO_WINDOWS
			return filenamepath.substr(filenamepath.find_last_of('\\') + 1);
#else
			return filenamepath.substr(filenamepath.find_last_of('/') + 1);
#endif
		}

	} // file
} // too
