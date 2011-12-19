// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef PAIR_H_INCL_x2ur832rx823r8r
#define PAIR_H_INCL_x2ur832rx823r8r

#include <utility>

namespace too
{
	namespace math
	{
		template<typename T1, typename T2> struct pair
		{
			pair() : m_p(), left(getleft()), right(getright()) {}
			pair(const T1& l, const T2& r) : m_p(l, r), left(getleft()), right(getright()) {}
			template<typename OT1, typename OT2>
			pair(const pair<OT1, OT2>& p) : m_p(p), left(getleft()), right(getright()) {}
			template<typename OT1, typename OT2>
			pair<T1, T2>& operator=(const pair<OT1, OT2>& p)
			{
				pair<T1, T2> temp(p);
				temp.swap(*this);
				return *this;
			}

			template<typename OT1, typename OT2>
			pair<T1, T2>& operator=(const std::pair<OT1, OT2>& p)
			{
				pair<T1, T2> temp(p);
				temp.swap(*this);
				return *this;
			}

			template<typename OT1, typename OT2>
			pair(const std::pair<OT1, OT2>& p) : m_p(p), left(getleft()), right(getright()) {}

			template<typename OT1, typename OT2>
			operator std::pair<OT1, OT2>() const
			{
				return m_p;
			}

			template<typename OT1, typename OT2>
			pair<T1, T2>& operator+=(const pair<OT1, OT2>& p)
			{
				m_p.first+= p.left;
				m_p.second+= p.right;
				return *this;
			}

			template<typename OT1, typename OT2>
			pair<T1, T2>& operator+=(const std::pair<OT1, OT2>& p)
			{
				m_p.first+= p.first;
				m_p.second+= p.second;
				return *this;
			}

			void swap(pair<T1, T2>& p) { m_p.swap(p); }

			pair<T1, T2> flip() { return pair<T1, T2>(m_p.second, m_p.first); }

			T1& left;
			T2& right;
		private:
			std::pair<T1, T2> m_p;
			T1& getleft() { return m_p.first; }
			T2& getright() { return m_p.second; }
		};
	} // math
} // too

#endif