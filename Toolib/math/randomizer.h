// Markus Borris, 2011
// This file is part of my Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef RANDOMIZER_H_INCL_238zrxn2rrfhf
#define RANDOMIZER_H_INCL_238zrxn2rrfhf

#include <cstdlib>
#include <sys/types.h>
#include <sys/timeb.h>
#include "../types.h"
#include "../log.h"
#include "../LOGDEF.h"

namespace too
{
	namespace math
	{
		//! Generator for pseudo random numbers.
		class randomizer
		{
		public:
			//! Reseeds the generator. Gets called automatically, when using next() or next_sign() the first time.
			static void reseed() {
				_timeb t;
				if (!_ftime_s(&t))
				{
					srand(t.millitm);
				}
				else
				{
					TOOLOGe("too::math::randomizer::reseed(): Seed error. _ftime_s() failed.");
				}
			}
			//! For testing reasons it could be useful to obtain the same sequence everytime.
			/** So, this gives the opportunity to manually seed as you like.*/
			static void reseed(uint seed)
			{
				srand(seed);
			}
			/** \returns a positive pseudo random number.
			\param min inclusive lower bound.
			\param max inclusive upper bound. Is also allowed to be larger than RAND_MAX.*/
			static inline u32 next(u32 min, u32 max)
			{
				static bool bSeeded = false;
				if (!bSeeded)
				{
					reseed();
					bSeeded = true;
				}
				return static_cast<u32>(rand()/(((double)RAND_MAX+1)/(double)(max-min+1)))+min;
			}
			//! \returns a pseudo random bool.
			static inline bool next()
			{
				return next(1, 100) % 2 == 0 ? true : false;
			}
			//! \returns a pseudo random sign, +1 or -1;
			static inline s8 next_sign()
			{
				return next()? 1 : -1;
			}
			/** \returns a positive floating point pseudo random number.
			\param min inclusive lower bound.
			\param max exclusive upper bound. Is also allowed to be larger than RAND_MAX.
			\param fracdigits number of fractional digits.*/
			static inline f64 next(u32 min, u32 max, u16 fracdigits)
			{
				f64 ret = 0.0;
				ret+= next(min, max-1);
				f64 div = 10.0;
				for (u16 i = 1; i <= fracdigits; ++i)
				{
					ret+= next(0, 9) / div;
					div*= 10.0;
				}
				return ret;
			}
			//! For use with STL algorithms.
			/** Usage: Declare "fpNEXTRANDOM rd = next_std" and use rd (function pointer) as functor.*/
			static ptrdiff_t next_std(ptrdiff_t excl_max)
			{
				return static_cast<ptrdiff_t>(next(0, static_cast<u32>(excl_max-1)));
			}
			typedef ptrdiff_t (*fpNEXTRANDOM)(ptrdiff_t);
		};
	} // math
} // too

#endif
