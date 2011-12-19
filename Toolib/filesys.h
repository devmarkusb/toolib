// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef FILESYS_H_INCL_xcsabgzduzbvfget32
#define FILESYS_H_INCL_xcsabgzduzbvfget32

#include <string>

namespace too
{
	namespace file
	{
		//! Hopefully not larger than int.
		int FileSize(const char* name);
		bool DelFile(const char* name);
		bool RenameFile(const char* oldname, const char* newname);
		std::string ExtractDirectory(const std::string& filenamepath);
		std::string ExtractFilename(const std::string& filenamepath);
	} // file
} // too

#endif