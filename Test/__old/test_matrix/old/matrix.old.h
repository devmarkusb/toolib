// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/** This is the discontinued(?) version containing a matrix class using Slice_iter's of a (in my
opinion) very inefficient type. The iterators still offer an inefficient index access, which
should be unnecessary. It would be easier and faster (I guess) to copy slices of rows or columns
into temporary valarray's instead. They have straight index access (incremental) for every desired
row or column. But perhaps I'm wrong and one assignment is slower than one multiplication.
Todo: test.
12.01.2011: Test done. Develop the class from scratch and as lowlevel as possible. The overhead using
valarrays etc. is unacceptable. Plan C-arrays work so much faster. I leave this file as the old code
just for educational purposes and rename class and file.
*/
//! \file

#pragma once
#ifndef MATRIX_H_INCL
#define MATRIX_H_INCL

#include <valarray>
#include <cmath>
#include "types.h"

namespace tl
{
	namespace math
	{
		//! Represents a mxn-matrix with m rows and n columns. T should be a reasonable basic numerical type.
		/** The index access is like (row i, column j):<br>
		(0,0) (0,1) (0,2) ... (0,n)	<br>
		(1,0) (1,1)  ...			<br>
		(2,0)  ...					<br>
		 ...						<br>
		(m,0)  ...			  (m,n)*/
		template <class T> class matrix : public std::valarray<T>
		{
		public:
			//! Constructor initialising the dimensions and entries. Default null matrix.
			matrix(size_t dim_rows, size_t dim_cols, bool bZeroize = false)
				: std::valarray<T>(dim_rows*dim_cols), m_dim_rows(dim_rows), m_dim_cols(dim_cols)
			{
				if (bZeroize)
					zeroize();
			}

			//! Nulls elements.
			void zeroize() {
				for (size_t i = 0; i < size(); ++i)
					(*this)[i] = T();
			}
			//! Get row dimension.
			size_t GetRowDim() const { return m_dim_rows; }
			//! Get column dimension.
			size_t GetColDim() const { return m_dim_cols; }
			//! Matrix entry access
			T& operator()(size_t row, size_t column) { return (*this)[column+row*m_dim_cols]; }
			//{ return Row(row)[column]; }
			T operator()(size_t row, size_t column) const { return (*this)[column+row*m_dim_cols]; }
			//{ return Row(row)[column]; }
			//! Matrix muliplied with (column-)vector.
			/** No dimension check is performed, so pay attention that
			\param m1 is a mxk matrix (rows x columns)
			\param m2 is a kxn matrix,
			to
			\return a mxn matrix.*/
			friend matrix<T> operator*(const matrix<T>& m1, const matrix<T>& m2)
			{
				/*matrix<T> res(m1.GetRowDim(), m2.GetColDim());
				for (size_t i = 0; i < m1.GetRowDim(); ++i)
					for (size_t j = 0; j < m2.GetColDim(); ++j)
						res(i, j) = dotproduct(m1.Row(i), m2.Col(j));
				return res;*/
				size_t
					m1r = m1.GetRowDim(),
					m2c = m2.GetColDim(),
					m1c = m1.GetColDim();
				matrix<T> res(m1r, m2c);
				for (size_t i = 0; i < m1r; ++i)
					for (size_t j = 0; j < m2c; ++j)
					{
						T sum = T();
						for (size_t k = 0; k < m1c; ++k)
						{
							sum+= m1[k+i*m1c]*m2[j+k*m2c];
						}
						res(i, j) = sum;
					}
				return res;
			}

			//=========================================
			// Operations for quadratic matrices only.
			//=========================================
			//todo ... lots of operations for quadratic matrices

		private:
			// Auxiliary tools, iterators for a slice.
			class Slice_iter
			{
			private:
				std::valarray<T>* m_v;
				const std::slice m_s;
				std::valarray<T> m_slicedv;
				size_t m_current;
				T& m_elem(size_t i) //{ return (*m_v)[m_s.start()+i*m_s.stride()]; }
				{ return m_slicedv[i]; }
			public:
				Slice_iter(std::valarray<T>* v, const std::slice s)
					: m_v(v), m_s(s), m_slicedv((*m_v)[m_s]), m_current(0){}
				Slice_iter end() const {
					Slice_iter t = *this; t.m_current = m_s.size(); return t;
				}
				Slice_iter& operator++() { m_current++; return *this; }
				Slice_iter operator++(int) { Slice_iter t = *this; m_current++; return t; }
				T& operator[](size_t i) { return m_elem(i); }
				T& operator*() { return m_elem(m_current); }
				friend bool operator==(const Slice_iter& si1, const Slice_iter& si2) {
					return si1.m_current==si2.m_current && si1.m_s.stride()==si2.m_s.stride()
						&& si1.m_s.start()==si2.m_s.start();
				}
				friend bool operator!=(const Slice_iter& si1, const Slice_iter& si2) {
					return !(si1==si2);
				}
				friend bool operator< (const Slice_iter& si1, const Slice_iter& si2) {
					return si1.m_current<si2.m_current && si1.m_s.stride()==si2.m_s.stride()
						&& si1.m_s.start()==si2.m_s.start();
				}
			}; // Slice_iter
			class Slice_citer
			{
			private:
				const std::valarray<T>* m_v;
				const std::slice m_s;
				const std::valarray<T> m_slicedv;
				size_t m_current;
				T m_elem(size_t i) const // { return (*m_v)[m_s.start()+i*m_s.stride()]; }
				{ return m_slicedv[i]; }
			public:
				Slice_citer(const std::valarray<T>* v, const std::slice s)
					: m_v(v), m_s(s), m_slicedv((*m_v)[m_s]), m_current(0){}
				Slice_citer end() const {
					Slice_citer t = *this; t.m_current = m_s.size(); return t;
				}
				Slice_citer& operator++() { m_current++; return *this; }
				Slice_citer operator++(int) { Slice_citer t = *this; m_current++; return t; }
				T operator[](size_t i) const { return m_elem(i); }
				T operator*() const { return m_elem(m_current); }
				friend bool operator==(const Slice_citer& si1, const Slice_citer& si2) {
					return si1.m_current==si2.m_current && si1.m_s.stride()==si2.m_s.stride()
						&& si1.m_s.start()==si2.m_s.start();
				}
				friend bool operator!=(const Slice_citer& si1, const Slice_citer& si2) {
					return !(si1==si2);
				}
				friend bool operator< (const Slice_citer& si1, const Slice_citer& si2) {
					return si1.m_current<si2.m_current && si1.m_s.stride()==si2.m_s.stride()
						&& si1.m_s.start()==si2.m_s.start();
				}
				friend T dotproduct(Slice_citer v1, Slice_citer v2)
				{
					T res = T();
					for ( ; v1 != v1.end(); ++v1, ++v2)
						res+= (*v1)*(*v2);
					return res;
				}
			}; // Slice_citer

			// Row and column dimensions.
			size_t m_dim_rows, m_dim_cols;
			// Extract a specific whole row or column as iterator.
			Slice_iter Row(size_t i) { return Slice_iter(this, std::slice(i*m_dim_cols, m_dim_cols, 1)); }
			Slice_citer Row(size_t i) const { return Slice_citer(this, std::slice(i*m_dim_cols, m_dim_cols, 1)); }
			Slice_iter Col(size_t i) { return Slice_iter(this, std::slice(i, m_dim_rows, m_dim_cols)); }
			Slice_citer Col(size_t i) const { return Slice_citer(this, std::slice(i, m_dim_rows, m_dim_cols)); }
		}; // matrix

		typedef matrix<f32> matrixf; // float
		typedef matrix<f64> matrixd; // double
	} // math
} // tl

#endif