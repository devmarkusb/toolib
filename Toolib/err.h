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

namespace too
{
	namespace err
	{
		//!
		std::string getTypename(const std::exception& e);

		//! Objects of this class can be thrown with some text information.
		class CException_info : virtual public std::exception
		{
			std::string m_s;
		public:
			CException_info(const std::string& s = "") : m_s(s) {}
			const std::string& getInfo() const { return m_s; }
		};
	} // err
} // too

#endif
