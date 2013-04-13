// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef NUMBER_H_INCL_f29jh8hnf238hrxz23
#define NUMBER_H_INCL_f29jh8hnf238hrxz23

#include "types.h"

namespace too
{
	namespace math
	{
		namespace NSB
		{
			enum : byte {
				BIN	= 2,
				OCT	= 8,
				DEC	= 10,
				HEX	= 16,
			};
		}
		typedef byte numsys_base;

		inline uint getDecDigitCount(uint iDecNumber, numsys_base tBase = NSB::DEC)
		{
			uint count = 0;
			do
			{
				++count;
				iDecNumber/= tBase;
			}
			while (iDecNumber != 0);
			return count;
		}
	}
}

#endif
