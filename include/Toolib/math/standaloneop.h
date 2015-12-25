// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/** Implements stand-alone operations +,-,*,/ for two objects of the same arbitrary type T.
    This is done in terms of +=,-=,*=,/= that have to be supported by T, as well as
	a copy constructor.
	Be careful not to spoil the global namespace with a using directive of namespace stand_alone_ops.
*/
//! \file


#ifndef STANDALONEOP_H_INCL_98243rz823xr
#define STANDALONEOP_H_INCL_98243rz823xr

namespace too
{
	namespace math
	{
		namespace stand_alone_ops
		{
			//!
			template <typename T> const T operator+(const T& t1, const T& t2)
			{
				return T(t1)+= t2;
			}
			//!
			template <typename T> const T operator-(const T& t1, const T& t2)
			{
				return T(t1)-= t2;
			}
			//!
			template <typename T> const T operator*(const T& t1, const T& t2)
			{
				return T(t1)*= t2;
			}
			//!
			template <typename T> const T operator/(const T& t1, const T& t2)
			{
				return T(t1)/= t2;
			}
		}
	}
}

#endif
