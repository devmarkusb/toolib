// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef STR2WSTR_H_INCL_827rt82rtrxjr38t
#define STR2WSTR_H_INCL_827rt82rtrxjr38t

#include "../PPDEFS.h"

#include <string>
#include <limits>
#if TOO_WINDOWS
#include <winnls.h>
#endif TOO_WINDOWS

namespace too
{
	namespace str
	{
		// fwds (implementation details, do not use from the outside)
		namespace str_impl
		{
#if TOO_WINDOWS
			inline std::wstring s2ws_windows(const std::string&);
#endif TOO_WINDOWS
			inline std::wstring s2ws_std(const std::string&);
		}

		//! Convert string to wstring.
		inline std::wstring s2ws(const std::string& s)
		{
#if TOO_WINDOWS
			return str_impl::s2ws_windows(s);
#else TOO_WINDOWS
			return str_impl::s2ws_std(s);
#endif TOO_WINDOWS
		}

		namespace str_impl
		{
#if TOO_WINDOWS
			inline std::wstring s2ws_windows(const std::string& s)
			{
				int slength = static_cast<int>(s.length()) + 1;
				if (slength < 0)
				{
#pragma push_macro("max")
#undef max
					slength = std::numeric_limits<int>::max();
#pragma pop_macro("max")
				}
				int len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
				wchar_t* buf = new wchar_t[len];
				try
				{
					MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
					std::wstring r(buf);
					delete[] buf;
					return r;
				}
				catch (...)
				{
					delete[] buf;
					return L"";
				}
			}
#endif TOO_WINDOWS
			inline std::wstring s2ws_std(const std::string& s)
			{
				std::wstring ws(s.length(), L' ');
				std::copy(s.begin(), s.end(), ws.begin());
				return ws;
			}
		}
	}
}
#endif
