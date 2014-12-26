// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#ifndef ERR_H_INCL_eeicgjm5iewtio4euih54
#define ERR_H_INCL_eeicgjm5iewtio4euih54

#include <exception>
#include <string>
#include "ToolibDEF.h"


namespace too
{
namespace err
{
//!
TOO_EXTERN_C_DECLS_BEGIN
TOOLIBSHARED_EXPORT std::string getTypename(const std::exception& e);
TOO_EXTERN_C_DECLS_END
}
}


#endif
