// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef VECTOR2D_H_INCL
#define VECTOR2D_H_INCL

#include <valarray>
#include <cmath>
#include "types.h"

namespace too
{
	namespace math
	{
		//! Represents a 2-dimensional vector. T should be a reasonable basic numerical type.
		/** The index access starts at 0 for the first coordinate.*/
		template <class T> class vector2d : public std::valarray<T>
		{
		private:
			static const size_t DIM = 2; // just to have a meaningful appearance of the number in the code
		public:
			//! Constructor initialising the coordinates. Default null vector.
			vector2d(const T& x1 = T(), const T& x2 = T()) : std::valarray<T>(DIM) {
				set(x1, x2);
			}
			//! Constructor for compatibilty reasons.
			vector2d(const std::valarray<T>& x) : std::valarray<T>(x){}

			//! Set coordinates.
			void set(const T& x1, const T& x2) { (*this)[0] = x1; (*this)[1] = x2; }
			//! Get euclidean length.
			T length() const {
				const T& x1 = (*this)[0]; const T& x2 = (*this)[1];
				return sqrt(x1*x1+x2*x2);
			}
			//! Get euclidean length squared.
			T lengthsq() const {
				const T& x1 = (*this)[0]; const T& x2 = (*this)[1];
				return x1*x1+x2*x2;
			}
			//todo dot product, normalize, distancefrom, rotateby, getangle, setlength
		}; // vector2d

		typedef vector2d<f32> vector2df; // float
		typedef vector2d<f64> vector2dd; // double
	} // math
} // tl

#endif