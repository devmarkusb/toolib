// Markus Borris, 2011
// This file is part of Toolib library. Open source.

//!
/**
*/
//! \file


#ifndef VECTOR_H_INCL_c8x3u923z4zr87r2
#define VECTOR_H_INCL_c8x3u923z4zr87r2

#include <cmath>


namespace too
{
	namespace math
	{
		//! Represents an n-dimensional vector. T should be a reasonable basic numerical type.
		/** The index access starts at 0 for the first coordinate.
		For very large (high DIM) vectors use matrix instead (danger of stack overflow).*/
		template <typename T, uint32_t DIM> class vector
		{
		private:
			//! Allocates memory for the vector data.
			void alloc() { x = new T[DIM]; }
			//! Frees memory of the vector data.
			void free() { delete[] x; }
			//! Swap for copy-and-swap assignment.
			void swap(vector& v)
			{
				std::swap(this->x, v.x);
			}
		public:
			//!
			class error_division_by_zero : virtual public std::exception{};

			//! Data elements of the vector.
			T* x;

			//! Constructor leaving coordinates undefined.
			/** \param bZeroize initializes with zeros when true.*/
			explicit vector(bool bZeroize = false)
			{
				alloc();
				if (bZeroize)
				{
					T zero = T();
					for (uint32_t i = 0; i < DIM; ++i)
						x[i] = zero;
				}
			}
			//! Constructor initialising the coordinates.
			explicit vector(const T ax[DIM])
			{
				alloc();
				for (uint32_t i = 0; i < DIM; ++i)
					x[i] = ax[i];
			}
			//! Copy, for the same type.
			vector(const vector& v)
			{
				alloc();
				for (uint32_t i = 0; i < DIM; ++i)
					x[i] = v.x[i];
			}
			//! Assignment, for the same type.
			vector& operator=(const vector& v)
			{
				if (this != &v)
					vector(v).swap(*this);
				return *this;
			}
			//! Frees memory.
			~vector()
			{
				try { free(); }
				catch (...) {}
			}

			//! Copy, for different type.
			template <class U> vector(const vector<U, DIM>& v)
			{
				alloc();
				for (uint32_t i = 0; i < DIM; ++i)
					x[i] = v.x[i];
			}
			//! Assignment, for different type.
			template <class U> vector& operator=(const vector<U, DIM>& v)
			{
				// naive test for self-assignment doesn't compile
				vector<T, DIM>(v).swap(*this);
				return *this;
			}

			/* No longer needed, implicit casting is now provided by constructor and assignment.
			//! Casts this vector<T> to a differently "typed" vector<T2>.
			/** Works if conversion from T to T2 exists. The dimensions have to be the same.
			template <class T2> vector<T2, DIM> vector_cast()
			{
				vector<T2, DIM> ret;
				for (uint32_t i = 0; i < DIM; ++i)
					ret.x[i] = x[i];
				return ret;
			}*/

			//! Index access operator. Simplifies access for vector v from v.x[i] to v[i].
			/** Within loops it is probably faster to access directly via a pre-specified T* temp=&v.x[0].*/
			T& operator[](uint32_t i) { return x[i]; }
			//! For rare cases of confusion, get back the actual dimension of the type.
			uint32_t getDIM() const { return DIM; }

			//! Dot product.
			friend T dot_product(const vector& v1, const vector& v2)
			{
				T res = T();
				for (uint32_t i = 0; i < DIM; ++i)
					res+= v1.x[i]*v2.x[i];
				return res;
			}
			//! Scalar multiplication.
			friend vector operator*(const vector& v, const T& t)
			{
				vector res;
				for (uint32_t i = 0; i < DIM; ++i)
					res.x[i] = v.x[i]*t;
				return res;
			}
			//! Scalar multiplication.
			friend vector operator*(const T& t, const vector& v)
			{
				vector res;
				for (uint32_t i = 0; i < DIM; ++i)
					res.x[i] = t*v.x[i];
				return res;
			}
			//! Scalar reciprocal multiplication.
			/** Throws error_division_by_zero, where t==T() is taken as zero.*/
			friend vector operator/(const vector& v, const T& t)
			{
				if (t == T())
					throw error_division_by_zero();
				vector res;
				for (uint32_t i = 0; i < DIM; ++i)
					res[i] = v.x[i]/t;
				return res;
			}
			//! Scalar reciprocal multiplication.
			/** Throws error_division_by_zero, where v[.]==T() is taken as "zero".*/
			friend vector operator/(const T& t, const vector& v)
			{
				vector res;
				for (uint32_t i = 0; i < DIM; ++i)
				{
					if (v.x[i] == T())
						throw error_division_by_zero();
					res.x[i] = t/v.x[i];
				}
				return res;
			}
			//! Scalar multiplication.
			vector& operator*=(const T& t)
			{
				for (uint32_t i = 0; i < DIM; ++i)
					x[i]*= t;
				return *this;
			}
			//! Scalar reciprocal multiplication.
			/** Throws error_division_by_zero, where t==T() is taken as zero.*/
			vector& operator/=(const T& t)
			{
				if (t == T())
					throw error_division_by_zero();
				for (uint32_t i = 0; i < DIM; ++i)
					x[i]/= t;
				return *this;
			}
			//! Sum.
			vector& operator+=(const vector& v)
			{
				for (uint32_t i = 0; i < DIM; ++i)
					x[i]+= v.x[i];
				return *this;
			}
			//! Difference.
			vector& operator-=(const vector& v)
			{
				for (uint32_t i = 0; i < DIM; ++i)
					x[i]-= v.x[i];
				return *this;
			}
			//! Comparison.
			friend bool operator==(const vector& v1, const vector& v2)
			{
				for (uint32_t i = 0; i < DIM; ++i)
				{
					if (v1.x[i] != v2.x[i])
						return false;
				}
				return true;
			}
			//! Comparison.
			friend bool operator!=(const vector& v1, const vector& v2)
			{
				return !(v1==v2);
			}

			//! Get euclidean length.
			T length() const {
				T r = T();
				for (uint32_t i = 0; i < DIM; ++i)
					r+= x[i]*x[i];
				return sqrt(r);
			}
			//! Get euclidean length squared.
			T lengthsq() const {
				T r = T();
				for (uint32_t i = 0; i < DIM; ++i)
					r+= x[i]*x[i];
				return r;
			}

			//todo normalize, distancefrom, rotateby, getangle, setlength

		}; // vector

		//! 2D specific vector for easier initialization or special operations or just using the name.
		template <class T> class vector2d : public vector<T, 2>
		{
		public:
			//! Constructor initialising the coordinates.
			explicit vector2d(const T& x0=T(), const T& x1=T()) : vector()
			{
				x[0] = x0; x[1] = x1;
			}
			//! Copy, for the same type.
			vector2d(const vector2d& v) : vector(v){}
			//! Assigment, for the same type.
			vector2d& operator=(const vector2d& v)
			{
				vector::operator=(v);
				return *this;
			}

			//! Copy, for different type.
			template <class U> vector2d(const vector2d<U>& v) : vector(v){}
			//! Assignment, for different type.
			template <class U> vector2d& operator=(const vector2d<U>& v)
			{
				vector::operator=(v);
				return *this;
			}

			//! For compatibility with base class.
			vector2d(const vector& v) : vector(v){}
			//! For compatibility with base class.
			vector2d& operator=(const vector& v)
			{
				vector::operator=(v);
				return *this;
			}

			//! Set coordinates.
			void set(const T& x0=T(), const T& x1=T())
			{
				x[0] = x0; x[1] = x1;
			}
		};

		//! 3D specific vector for easier initialization or special operations or just using the name.
		template <class T> class vector3d : public vector<T, 3>
		{
		public:
			//! Constructor initialising the coordinates.
			explicit vector3d(const T& x0=T(), const T& x1=T(), const T& x2=T()) : vector()
			{
				x[0] = x0; x[1] = x1; x[2] = x2;
			}
			//! Copy, for the same type.
			vector3d(const vector3d& v) : vector(v){}
			//! Assigment, for the same type.
			vector3d& operator=(const vector3d& v)
			{
				vector::operator=(v);
				return *this;
			}

			//! Copy, for different type.
			template <class U> vector3d(const vector3d<U>& v) : vector(v){}
			//! Assignment, for different type.
			template <class U> vector3d& operator=(const vector3d<U>& v)
			{
				vector::operator=(v);
				return *this;
			}

			//! For compatibility with base class.
			vector3d(const vector& v) : vector(v){}
			//! For compatibility with base class.
			vector3d& operator=(const vector& v)
			{
				vector::operator=(v);
				return *this;
			}

			//! Set coordinates.
			void set(const T& x0=T(), const T& x1=T(), const T& x2=T())
			{
				x[0] = x0; x[1] = x1; x[2] = x2;
			}
		};

		// 4D specific vector for easier initialization or special operations or just using the name.
		//todo
		/*template <class T> class vector4d : public vector<T, 4>
		{
		public:
			//! Constructor initialising the coordinates.
			explicit vector4d(const T& x0=T(), const T& x1=T(), const T& x2=T(), const T& x3=T())
				: x[0](x0), x[1](x1), x[2](x2), x[3](x3){}
		};
		*/
		//!
		template <typename T, uint32_t DIM> const vector<T, DIM>
		operator+(const vector<T, DIM>& t1, const vector<T, DIM>& t2)
		{
			return vector<T, DIM>(t1)+= t2;
		}
		//!
		template <typename T, uint32_t DIM> const vector<T, DIM>
		operator-(const vector<T, DIM>& t1, const vector<T, DIM>& t2)
		{
			return vector<T, DIM>(t1)-= t2;
		}

		typedef vector2d<float> vector2df;
		typedef vector2d<double> vector2dd;
		typedef vector3d<float> vector3df;
		typedef vector3d<double> vector3dd;
		/*typedef vector4d<float> vector4df;
		typedef vector4d<double> vector4dd;*/
	}
}

#endif
