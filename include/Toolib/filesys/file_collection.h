// Markus Borris, 2016
// This file is part of Toolib library.

//!
/**
*/
//! \file

#ifndef FILE_COLLECTION_H_sdhgfnxoi452178943tzx3t1gfn
#define FILE_COLLECTION_H_sdhgfnxoi452178943tzx3t1gfn

#include <string>
#include <vector>
#include "ToolibDEF.h"

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_4251_BEGIN"

namespace too
{

namespace file
{

class TOOLIBSHARED_EXPORT FileCollection
{
public:
    static const unsigned char max_digits = 10;

    //! \param base_file_name with path but without extension \param file_ext and without
    //! a potential number of a file collection (e.g. 0000..9999 - 4 digits here). These
    //! kind of variations of a base file name are automatically checked for existence
    //! and can be retrieved via get_list_of_existent_files().
    FileCollection(const std::string& base_file_name, const std::string& file_ext);

    std::vector<std::string> get_list_of_existent_files() const;

private:
    std::vector<std::string> file_list;

    unsigned char obtain_number_of_digits_for_filenames_of_file_collection(
        const std::string& base_file_name, const std::string& file_ext) const;
};
}
}

#include "Toolib/PPDefs/MSVC/SUPPRESS_WARNING_END"

#endif
