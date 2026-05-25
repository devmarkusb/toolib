//! \file

#ifndef FILE_COLLECTION_HPP_SDHGFNXOI452178943TZX3T1GFN
#define FILE_COLLECTION_HPP_SDHGFNXOI452178943TZX3T1GFN

#include "../config.hpp"
#include "mb/ul/buildenv/warnings.hpp"
#include <string>
#include <vector>

UL_PRAGMA_WARNINGS_PUSH
UL_WARNING_DISABLE_MSVC(4251)

namespace mb::too::file {
class FileCollection {
public:
    static const unsigned char max_digits = 10;

    /** Converts a fn filename to one without extension and number. Removes extension only, if filename contains
        nothing but numbers.*/
    static std::string get_base_name(const std::string& fn);

    /** \param file_name is a path of a file that could be part of a collection of files ending
        with a number (e.g. 0000..9999 - 4 digits here). The numbering has to start with 0 or 00
        or 000, ... and so on. Otherwise only the given file can be retrieved via get_list_of_existent_files().
        These kind of variations of a base file name (e.g. base00, base01, base02, base03)
        are automatically checked for existence and can be retrieved via get_list_of_existent_files().*/
    explicit FileCollection(const std::string& file_name);

    //! \return all files found to be in the collection, or just a single file.
    [[nodiscard]] std::vector<std::string> get_list_of_existent_files() const;

private:
    std::vector<std::string> file_list_;

    [[nodiscard]] static unsigned char obtain_number_of_digits_for_filenames_of_file_collection(
        const std::string& base_file_name, const std::string& file_ext);
};
} // namespace mb::too::file

UL_PRAGMA_WARNINGS_POP


#endif
