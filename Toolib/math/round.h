// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file

#pragma once
#ifndef ROUND_H_INCL_9mj238fz23nf8z
#define ROUND_H_INCL_9mj238fz23nf8z

#include <cmath>
#include <limits>

namespace too
{
	namespace math
	{
		inline double round(double r)
		{
			return (r > 0.0) ? floor(r + 0.5) : ceil(r - 0.5);
		}
		template<typename T> inline T round2(double r)
		{
			using std::numeric_limits;
			const double d = round(r);
			if (d > numeric_limits<T>::max())
				return numeric_limits<T>::max();
			else if (d < numeric_limits<T>::min())
				return numeric_limits<T>::min();
			return static_cast<T>(d);
		}
	}
}

#endif
