// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef NON_COPYABLE_H_INCL_imcutoiu34o5
#define NON_COPYABLE_H_INCL_imcutoiu34o5

namespace too
{
	//! Provides a shortcut ultility to declare classes that are supposed to forbid copying.
	/** Usage: \code class SomeClassThatShallForbidCopy : private non_copyable { /* ... *\/ };\endcode
	Note that the inheritance has to be qualified private.*/
	class non_copyable
	{
	protected:
		non_copyable(){}
		~non_copyable(){}
	private:
		//! Forbid copy construction.
		non_copyable(const non_copyable&);
		//! Forbid copy assignment.
		non_copyable& operator=(const non_copyable&);
	};
}

#endif