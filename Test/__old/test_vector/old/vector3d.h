// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef VECTOR3D_H_INCL
#define VECTOR3D_H_INCL

#include <valarray>
#include <cmath>
#include "types.h"

namespace too
{
	namespace math
	{
		//! Represents a 3-dimensional vector. T should be a reasonable basic numerical type.
		/** The index access starts at 0 for the first coordinate.*/
		template <class T> class vector3d : public std::valarray<T>
		{
		private:
			static const size_t DIM = 3; // just to have a meaningful appearance of the number in the code
		public:
			//! Constructor initialising the coordinates. Default null vector.
			vector3d(const T& x1 = T(), const T& x2 = T(), const T& x3 = T()) : std::valarray<T>(DIM) {
				set(x1, x2, x3);
			}
			//! Constructor for compatibilty reasons.
			vector3d(const std::valarray<T>& x) : std::valarray<T>(x){}

			//! Set coordinates.
			void set(const T& x1, const T& x2, const T& x3) { (*this)[0] = x1; (*this)[1] = x2; (*this)[2] = x3; }
			//! Get euclidean length.
			T length() const {
				const T& x1 = (*this)[0]; const T& x2 = (*this)[1]; const T& x3 = (*this)[2];
				return sqrt(x1*x1+x2*x2+x3*x3);
			}
			//! Get euclidean length squared.
			T lengthsq() const {
				const T& x1 = (*this)[0]; const T& x2 = (*this)[1]; const T& x3 = (*this)[2];
				return x1*x1+x2*x2+x3*x3;
			}
			//todo dot product, cross product, normalize, distancefrom, rotatearound_x/y/z_axis_by, getangle, setlength
		}; // vector3d

		typedef vector3d<f32> vector3df; // float
		typedef vector3d<f64> vector3dd; // double
	} // math
} // tl

#endif