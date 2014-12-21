// Copyright (C) 2011 Markus Borris
// This file is part of the Toolib library.
// For conditions of distribution and use, see copyright notice in toolib.h.

//!
/**
*/
//! \file

#pragma once
#ifndef MATRIX_H_INCL
#define MATRIX_H_INCL

#include <cmath>
#include <string>
#include "types.h"
#include "error.h"

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
		template <class T> class matrix
		{
		public:
			//! Constructor initialising the dimensions and entries.
			/** \param bZeroize creates a null matrix when set to true. Default is false, meaning undefined entries.*/
			matrix(u32 dim_rows, u32 dim_cols, bool bZeroize = false)
				: m_dim_rows(dim_rows), m_dim_cols(dim_cols)
			{
				m_m = new T*[m_dim_rows];
				for (u32 i = 0; i < m_dim_rows; ++i)
					m_m[i] = new T[m_dim_cols];
				if (bZeroize)
					zeroize();
			}
			matrix(matrix& m) : m_dim_rows(m.m_dim_rows), m_dim_cols(m.m_dim_cols), m_m(m.m_m)
			{
				m.m_m = 0;
				m.m_dim_cols = 0;
				m.m_dim_rows = 0;
			}
			//! Frees the memory allocated for the internally stored matrix data.
			~matrix()
			{
				if (!m_m)
					return;
				for (u32 i = 0; i < m_dim_rows; ++i)
				{
					if (!m_m[i])
						continue;
					delete[] m_m[i];
					m_m[i] = 0;
				}
				delete[] m_m;
				m_m = 0;
			}

			//! Nulls elements.
			void zeroize() {

				for (u32 i = 0; i < m_dim_rows; ++i)
					for (u32 j = 0; j < m_dim_cols; ++j)
						m_m[i][j] = T();
			}
			//! Get row dimension.
			u32 RowCount() const { return m_dim_rows; }
			//! Get column dimension.
			u32 ColCount() const { return m_dim_cols; }
			//! Matrix entry access
			//T* operator[](u32 row) { return m_m[row]; }
			//! Matrix entry access
			//const T* operator[](u32 row) const { return m_m[row]; }
			//! Matrix entry access
			T& operator()(u32 row, u32 column) {
				try { return m_m[row][column]; }
				catch (...) { throw error_accessviolation(); }
			}
			//! Matrix entry access
			const T& operator()(u32 row, u32 column) const {
				try { return m_m[row][column]; }
				catch (...) { throw error_accessviolation(); }
			}
			//! Matrix muliplied with (column-)vector.
			/** No dimension check is performed, so pay attention that
			\param m1 is a mxk matrix (rows x columns)
			\param m2 is a kxn matrix,
			to
			\return a mxn matrix.*/
			friend matrix<T> operator*(const matrix<T>& m1, const matrix<T>& m2)
			{
				u32
					m1r = m1.m_dim_rows,
					m2c = m2.m_dim_cols,
					m1c = m1.m_dim_cols;
				matrix<T> res(m1r, m2c);
				try
				{
					for (u32 i = 0; i < m1r; ++i)
					{
						for (u32 j = 0; j < m2c; ++j)
						{
							T sum = T();
							for (u32 k = 0; k < m1c; ++k)
							{
								sum+= m1.m_m[i][k]*m2.m_m[k][j];
							}
							res.m_m[i][j] = sum;
						}
					}
					return res;
				}
				catch (...) { throw error_accessviolation(); }
			}

			//=========================================
			// Operations for quadratic matrices only.
			//=========================================
			//todo ... lots of operations for quadratic matrices

		private:
			// Row and column dimensions.
			u32 m_dim_rows, m_dim_cols;
			// The internal matrix data.
			T** m_m;
			/*// Extract a specific whole row or column as iterator.
			Slice_iter Row(u32 i) { return Slice_iter(this, std::slice(i*m_dim_cols, m_dim_cols, 1)); }
			Slice_citer Row(u32 i) const { return Slice_citer(this, std::slice(i*m_dim_cols, m_dim_cols, 1)); }
			Slice_iter Col(u32 i) { return Slice_iter(this, std::slice(i, m_dim_rows, m_dim_cols)); }
			Slice_citer Col(u32 i) const { return Slice_citer(this, std::slice(i, m_dim_rows, m_dim_cols)); }*/
		}; // matrix

		typedef matrix<f32> matrixf; // float
		typedef matrix<f64> matrixd; // double
	} // math
} // tl

#endif