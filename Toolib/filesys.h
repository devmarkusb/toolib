// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef FILESYS_H_INCL_xcsabgzduzbvfget32
#define FILESYS_H_INCL_xcsabgzduzbvfget32

#include <string>
#include "types.h"
#include "class/non_copyable.h"

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


        //#############################################################################################################

        class IFileSys : private too::non_copyable
        {
        public:
            static IFileSys* GetInstance();

            virtual void SaveToTextFile(const too::string& FilePathNameExt, const too::string& Content) = 0;
            virtual void LoadFromTextFile(const too::string& FilePathNameExt, too::string& Content) = 0;
            virtual void CopyFile(const too::string& FilePathNameExt_From, const too::string& FilePathNameExt_To) = 0;
            virtual void DeleteFile(const too::string& FilePathNameExt) = 0;
            virtual void RenameFile(const too::string& FilePathNameExt) = 0;
            virtual void CreateFolder(const too::string& FolderPath) = 0;
            virtual void DeleteFolder(const too::string& FolderPath) = 0;
            virtual void RenameFolder(const too::string& FolderPath) = 0;
            virtual bool FolderExists(const too::string& FolderPath) = 0;
            virtual bool FileExists(const too::string& FilePathNameExt) = 0;

            enum class ESysPathType
            {
                PROGRAM,
                USER,
                PROGDATA,
            };
            virtual void GetSystemPath(ESysPathType Type, too::string& Path) = 0;
        };


        //#############################################################################################################

        class IFileData
        {
        public:
            virtual ~IFileData() = default;

            virtual void SaveToFile(const too::string& FilePathNameExt) = 0;
            virtual void LoadFromFile(const too::string& FilePathNameExt) = 0;
        };
    } // file

    using IFileSys = file::IFileSys;
    using IFileData = file::IFileData;
} // too

#endif
