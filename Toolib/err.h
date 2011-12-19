// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef ERR_H_INCL_eeicgjm5iewtio4euih54
#define ERR_H_INCL_eeicgjm5iewtio4euih54

#include <exception>
#include <string>

namespace too
{
	namespace err
	{
		std::string getTypename(const std::exception& e);
	} // err
} // too

//todo need sth. like this:
//#include <string>
//#include <sstream> 
////! A simple exception class
///*!
//	The key of this exception is the ability to store text. 
//	The user is able to pump custom infos into the exception object using the << operator, e.g.
//	int i = 666; // holds the error code.
//	throw (DefaultException () << "The following error occured: " << i);
//*/
//class DefaultException
//{
//public:
//	//! Constructs an empty exception object.
//	DefaultException (){}
//	virtual ~DefaultException (){}
//	//! Use this operator to pump some additional infos into the exception text.
//	template <typename T>
//	inline DefaultException & operator<< (const T & stream)
//	{
//		std::stringstreamT ss;
//		ss << stream;
//		text_ += ss.str ().c_str ();
//		return *this;
//	}
//	//! Returns the text of the default exeption.
//	/*!
//		\return char const * A pointer to the text the exception holds.
//	*/
//	TCHAR const * what () const
//	{
//		return text_.c_str ();
//	}
//
//protected:
//	//! Holds the exception text.
//	std::stringT text_;
//};

#endif
