// 2011-16

//! \file

#ifndef MATRIX_H_mx02983urx23
#define MATRIX_H_mx02983urx23

#include "../config.h"
#include "ul/ul.h"
#include <cmath>
#include <complex>
#include <cstdint>
#include <string>


namespace mb::too::math
{
//! Represents a mxn-matrix with m rows and n columns. T should be a reasonable basic numerical type.
/** The index access is like (row i, column j):<br>
(0,0) (0,1) (0,2) ... (0,n)	<br>
(1,0) (1,1)  ...			<br>
(2,0)  ...					<br>
 ...						<br>
(m,0)  ...			  (m,n) <br>
Note that this class doesn't throw any own exceptions (except divisions by zero).
You have to take care about ranges.*/
// Implementation uses intrusive reference counting (idiom).
template <class T>
class matrix
{
public:
    class error_division_by_zero : virtual public std::exception
    {
    };
    class error_division_by_zero_det : virtual public error_division_by_zero
    {
    };

    //! Constructor allocating memory for a matrix with the given dimensions.
    /** \param dim_rows Count of rows.
        \param dim_cols Count of columns.*/
    matrix(uint32_t dim_rows, uint32_t dim_cols)
        : m_rep(new MRep(dim_rows, dim_cols, 0))
    {
    }
    //! Constructor allocating memory for a matrix with the given dimensions plus initialization.
    /** \param dim_rows Count of rows.
        \param dim_cols Count of columns.
        \param mtrx A usual 2-dimensional C array initializing the matrix entries.*/
    matrix(uint32_t dim_rows, uint32_t dim_cols, T** mtrx)
        : m_rep(new MRep(dim_rows, dim_cols, mtrx))
    {
    }
    //! Copies from another matrix, using the same internal representation to speed things up.
    matrix(const matrix& mtrx)
    {
        ++(mtrx.m_rep->iRefCount);
        m_rep = mtrx.m_rep;
    }
    //! Replaces itself with another matrix, using the same internal representation to speed things up.
    matrix& operator=(const matrix& mtrx)
    {
        /* It is taken care of self-assignment in a logical manner. No need to
        write \code if (this == &mtrx) return *this; \endcode*/
        ++(mtrx.m_rep->iRefCount);
        free();
        m_rep = mtrx.m_rep;
        return *this;
    }
    /* Applying the idiom "Coercion by Member Template" doesn't work well with private representation
    so far... (perhaps sth. for later improvement)
    //! Copies from another matrix, using the same internal representation to speed things up.
    template <class U> matrix(const matrix<U>& mtrx)
    {
        ++(mtrx.m_rep->iRefCount);
        m_rep = mtrx.m_rep;
    }
    //! Replaces itself with another matrix, using the same internal representation to speed things up.
    template <class U> matrix& operator=(const matrix<U>& mtrx)
    {
        ++(mtrx.m_rep->iRefCount);
        free();
        m_rep = mtrx.m_rep;
        return *this;
    }*/
    //! Frees the memory allocated for the internally stored matrix data.
    ~matrix()
    {
        try
        {
            free();
        }
        catch (...)
        {
        }
    }

    //! Cast this matrix<T> to a differently "typed" matrix<T2>. Works if conversion from T to T2 exists.
    template <class T2>
    matrix<T2> matrix_cast() const
    {
        uint32_t rows = m_rep->dim_rows, cols = m_rep->dim_cols;
        T** pptm = m_rep->m;
        matrix<T2> ret(rows, cols);
        T2** ppt2m = ret;
        for (uint32_t i = 0; i < rows; ++i)
            for (uint32_t j = 0; j < cols; ++j)
                ppt2m[i][j] = pptm[i][j];
        return ret;
    }

    //! Replaces itself with another matrix given as usual 2-dimensional C array.
    /** \param mtrx has to have the same dimensions as this internal matrix. Otherwise you should
    use a constructor instead. If mtrx is 0 nothing happens.*/
    matrix& assign(T** mtrx)
    {
        if (!mtrx)
            return *this;
        m_rep = m_rep->get_own_copy(false);
        T** m = m_rep->m;
        uint32_t rows = m_rep->dim_rows, cols = m_rep->dim_cols;
        for (uint32_t i = 0; i < rows; ++i)
        {
            for (uint32_t j = 0; j < cols; ++j)
                m[i][j] = mtrx[i][j];
        }
        return *this;
    }
    //! Nulls elements. Fast, if internal representation is referenced only once.
    void zeroize()
    {
        m_rep = m_rep->get_own_copy(false);
        T** m = m_rep->m;
        uint32_t rows = m_rep->dim_rows, cols = m_rep->dim_cols;
        T init = T();
        for (uint32_t i = 0; i < rows; ++i)
            for (uint32_t j = 0; j < cols; ++j)
                m[i][j] = init;
    }
    //! Checks if this matrix is zero.
    [[nodiscard]] bool isZero() const
    {
        T** m = m_rep->m;
        uint32_t rows = m_rep->dim_rows, cols = m_rep->dim_cols;
        T init = T();
        for (uint32_t i = 0; i < rows; ++i)
            for (uint32_t j = 0; j < cols; ++j)
                if (m[i][j] != 0)
                    return false;
        return true;
    }
    //! Get row dimension.
    [[nodiscard]] uint32_t RowCount() const
    {
        return m_rep->dim_rows;
    }
    //! Get column dimension.
    [[nodiscard]] uint32_t ColCount() const
    {
        return m_rep->dim_cols;
    }
    // Smart equivalent of T&
    class Tref;
    //! Matrix entry access. Medium reading, slow writing - do not use it in loops.
    Tref operator()(uint32_t row, uint32_t column)
    {
        return Tref(*this, row, column);
    }
    //! Matrix entry access. Medium reading - do not use it in loops.
    const T& operator()(uint32_t row, uint32_t column) const
    {
        return m_rep->m[row][column];
    }
    //! Matrix entry access by cast to T**. Fast version, for usage in loops.
    /** Two drawbacks: First of all, this provides low-level access to the private matrix data.
    And secondly, the cast assumes that the matrix content will be changed. Hence it starts
    by creating a new matrix representation, *if* the representation was not used only once thus far.*/
    explicit operator T**()
    {
        m_rep = m_rep->get_own_copy();
        return m_rep->m;
    }
    //! Scalar multiplying a matrix.
    friend matrix<T> operator*(const matrix<T>& m, const T& t)
    {
        uint32_t m1r = m.m_rep->dim_rows, m1c = m.m_rep->dim_cols;
        matrix<T> res(m1r, m1c);
        T** m1elem = m.m_rep->m;
        T** reselem = res.m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                reselem[i][j] = m1elem[i][j] * t;
            }
        }
        return res;
    }
    //! Scalar multiplying a matrix.
    friend matrix<T> operator*(const T& t, const matrix<T>& m)
    {
        uint32_t m1r = m.m_rep->dim_rows, m1c = m.m_rep->dim_cols;
        matrix<T> res(m1r, m1c);
        T** m1elem = m.m_rep->m;
        T** reselem = res.m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                reselem[i][j] = t * m1elem[i][j];
            }
        }
        return res;
    }
    //! Scalar reciprocal multiplying a matrix.
    /** Throws error_division_by_zero, where m[.][.]==T() is taken as "zero".*/
    friend matrix<T> operator/(const T& t, const matrix<T>& m)
    {
        uint32_t m1r = m.m_rep->dim_rows, m1c = m.m_rep->dim_cols;
        matrix<T> res(m1r, m1c);
        T** m1elem = m.m_rep->m;
        T** reselem = res.m_rep->m;
        T zero = T();
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                if (m1elem[i][j] == zero)
                    throw error_division_by_zero();
                reselem[i][j] = t / m1elem[i][j];
            }
        }
        return res;
    }
    //! Scalar reciprocal multiplying a matrix.
    /** Throws error_division_by_zero, where t==T() is taken as zero.*/
    friend const matrix<T> operator/(const matrix<T>& m, const T& t)
    {
        if (t == T())
            throw error_division_by_zero();
        uint32_t m1r = m.m_rep->dim_rows, m1c = m.m_rep->dim_cols;
        matrix<T> res(m1r, m1c);
        T** m1elem = m.m_rep->m;
        T** reselem = res.m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                reselem[i][j] = m1elem[i][j] / t;
            }
        }
        return res;
    }
    //! Matrix multiplication.
    /** Remark: Haven't yet found out why \code matrixd m3(0, 0); m3 = m1*m2; \endcode
    is fast as usual, whereas \code matrixd m3(m1*m2); \endcode is two times slower. The sheer
    existence of operator*= is not the reason (checked it).
    No dimension check is performed, so pay attention that
    this matrix is a mxk matrix (rows x columns) and
    \param m is a kxn matrix,
    to
    \return a mxn matrix.*/
    matrix& operator*=(const matrix& m)
    {
        uint32_t m1r = m_rep->dim_rows, m2c = m.m_rep->dim_cols, m1c = m_rep->dim_cols;
        matrix<T> res(m1r, m2c);
        T** m1elem = m_rep->m;
        T** m2elem = m.m_rep->m;
        T** reselem = res.m_rep->m;
        T sum;
        T* m1elemr;
        T init = T(); // eliminating every single function call and address jumping from the loops
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m2c; ++j)
            {
                sum = init;
                m1elemr = m1elem[i]; // speeds thing up tremendously
                for (uint32_t k = 0; k < m1c; ++k)
                {
                    sum += m1elemr[k] * m2elem[k][j];
                }
                reselem[i][j] = sum;
            }
        }
        *this = res;
        return *this;
    }
    //! Scalar multiplying this matrix.
    matrix& operator*=(const T& t)
    {
        uint32_t m1r = m_rep->dim_rows, m1c = m_rep->dim_cols;
        T** m1elem = m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                m1elem[i][j] *= t;
            }
        }
        return *this;
    }
    //! Scalar reciprocal multiplying this matrix.
    /** Throws error_division_by_zero, where t==T() is taken as zero.*/
    matrix& operator/=(const T& t)
    {
        if (t == T())
            throw error_division_by_zero();
        uint32_t m1r = m_rep->dim_rows, m1c = m_rep->dim_cols;
        T** m1elem = m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                m1elem[i][j] /= t;
            }
        }
        return *this;
    }
    //! Another matrix is added this one. Both have to coincide dimensionally.
    matrix& operator+=(const matrix& m)
    {
        uint32_t m1r = m_rep->dim_rows, m1c = m_rep->dim_cols;
        T** m1elem = m_rep->m;
        T** m2elem = m.m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                m1elem[i][j] += m2elem[i][j];
            }
        }
        return *this;
    }
    //! Another matrix is substracted from this one. Both have to coincide dimensionally.
    matrix& operator-=(const matrix& m)
    {
        uint32_t m1r = m_rep->dim_rows, m1c = m_rep->dim_cols;
        T** m1elem = m_rep->m;
        T** m2elem = m.m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                m1elem[i][j] -= m2elem[i][j];
            }
        }
        return *this;
    }
    //! Comparison of two matrices.
    friend bool operator==(const matrix<T>& m1, const matrix<T>& m2)
    {
        uint32_t m1r = m1.m_rep->dim_rows, m1c = m1.m_rep->dim_cols, m2r = m2.m_rep->dim_rows, m2c = m2.m_rep->dim_cols;
        if (m1r != m2r || m1c != m2c)
            return false;
        T** m1elem = m1.m_rep->m;
        T** m2elem = m2.m_rep->m;
        for (uint32_t i = 0; i < m1r; ++i)
        {
            for (uint32_t j = 0; j < m1c; ++j)
            {
                if (m1elem[i][j] != m2elem[i][j])
                    return false;
            }
        }
        return true;
    }
    //! Comparison of two matrices.
    friend bool operator!=(const matrix<T>& m1, const matrix<T>& m2)
    {
        return m1 != m2;
    }
    //! Returns the row-"matrix" (1xn) of the specified row of this matrix.
    matrix getrow(uint32_t row) const
    {
        uint32_t c = m_rep->dim_cols;
        matrix res(1, c);
        T** melem = m_rep->m;
        T** reselem = res.m_rep->m;
        for (uint32_t j = 0; j < c; ++j)
            reselem[0][j] = melem[row][j];
        return res;
    }
    //! Returns the column-"matrix" (mx1) of the specified column of this matrix.
    /** Due to data storage, this is slower than getrow(). Maybe one can consider transposition
    by transpose() first (if one needs to extract dozens of columns or those from the back of a huge
    matrix).*/
    matrix getcol(uint32_t col) const
    {
        uint32_t r = m_rep->dim_rows;
        matrix res(r, 1);
        T** melem = m_rep->m;
        T** reselem = res.m_rep->m;
        for (uint32_t i = 0; i < r; ++i)
            reselem[i][0] = melem[i][col];
        return res;
    }


    //### Operations for quadratic matrices only. ###

    //! Checks whether this matrix is the identity.
    /** Only applicable for quadratic matrices.
    \param bMakeIt true changes this matrix into identity. False leaves it unchanged.*/
    bool identity(bool bMakeIt = false)
    {
        uint32_t r = m_rep->dim_rows;
        T one = static_cast<T>(1.0);
        T zero = T();
        if (bMakeIt)
        {
            m_rep = m_rep->get_own_copy(false);
            T** elem = m_rep->m;
            for (uint32_t i = 0; i < r; ++i)
                for (uint32_t j = i; j < r; ++j)
                {
                    if (i == j)
                        elem[i][i] = one;
                    else
                    {
                        elem[i][j] = zero;
                        elem[j][i] = zero;
                    }
                }
        }
        else
        {
            T** elem = m_rep->m;
            for (uint32_t i = 0; i < r; ++i)
                for (uint32_t j = i; j < r; ++j)
                {
                    if (i == j)
                    {
                        if (elem[i][i] != one)
                            return false;
                    }
                    else if (elem[i][j] != zero || elem[j][i] != zero)
                        return false;
                }
        }
        return true;
    }
    //! Transposes this matrix. Works only for quadratic matrices.
    /** Of course this could have been implemented also for non-quadratic ones. But it would be
    easier to this externally if ever needed. Also there is no loss of performance, since a new
    allocation is needed in that case nevertheless.*/
    void transpose()
    {
        m_rep = m_rep->get_own_copy();
        uint32_t r = m_rep->dim_rows;
        T** elem = m_rep->m;
        // T aux;
        for (uint32_t i = 0; i < r; ++i)
            for (uint32_t j = i + 1; j < r; ++j)
            {
                std::swap(elem[i][j], elem[j][i]);
                /*aux = elem[i][j];
                elem[i][j] = elem[j][i];
                elem[j][i] = aux;*/
            }
    }
    //! Determinant.
    T det() const
    {
        throw ul::not_implemented{"det"};
    }
    //! Inverse. Throws error_division_by_zero_det exception if determinant is zero.
    void invert()
    {
        throw ul::not_implemented{"invert"};
    }
    //! Is invertible?
    [[nodiscard]] bool isInvertible() const
    {
        throw ul::not_implemented{"isInvertible"};
    }
    //! Is symmetric?
    [[nodiscard]] bool isSymmetric() const
    {
        throw ul::not_implemented{"isSymmetric"};
    }
    //! Is orthogonal?
    [[nodiscard]] bool isOrthogonal() const
    {
        throw ul::not_implemented{"isOrthogonal"};
    }
    //! Is diagonal?
    [[nodiscard]] bool isDiagonal() const
    {
        throw ul::not_implemented{"isDiagonal"};
    }


    //### Implementation details. ###

    //! Smart equivalent of T& (proxy class). Used to implement "index" access to the matrix entries.
    class Tref
    {
    private:
        friend class matrix;
        matrix& m;
        uint32_t r, c; // rows, cols
        Tref(matrix& mtrx, uint32_t row, uint32_t col)
            : m(mtrx)
            , r(row)
            , c(col)
        {
        }

    public:
        // matrix element is just read from, appears as simple T
        operator T() const
        {
            return m.m_rep->m[r][c];
        }
        // matrix element is written to
        void operator=(const T& t)
        {
            m.put(r, c, t);
        }
    };

protected:
    //! This was introduced mainly for inheriting classes to provide proper reference counting operations.
    void get_own_rep_copy(bool bInitialiseIfNeededNew = true)
    {
        m_rep = m_rep->get_own_copy(bInitialiseIfNeededNew);
    }

private:
    /* The internal representation of the matrix data. Many objects could share the same, which
    is taken track of by reference counting. Anytime a new or changed object appears, a new representation
    is allocated.*/
    struct MRep
    {
    public:
        // Row and column dimensions.
        uint32_t dim_rows, dim_cols;
        // The internal matrix data.
        T** m;
        // The reference counter.
        mutable int32_t iRefCount;

        // Allocating a brand new single representation.
        MRep(uint32_t rows, uint32_t cols, T** mtrx)
            : iRefCount(1)
            , dim_rows(rows)
            , dim_cols(cols)
            , m(new T*[rows])
        {
            for (uint32_t i = 0; i < rows; ++i)
            {
                m[i] = new T[cols];
                if (mtrx)
                    for (uint32_t j = 0; j < cols; ++j)
                        m[i][j] = mtrx[i][j];
            }
        }
        MRep(const MRep&) = delete;
        MRep& operator=(const MRep&) = delete;
        // The representation is no longer needed (normally a consequence of the counter approaching 0).
        ~MRep()
        {
            try
            {
                free();
            }
            catch (...)
            {
            }
        }

        /* This method is usually called when someone wants to change the matrix. If the represention
        is only single, changing is immediately allowed. If it is shared twice or more often, it has to
        be extracted to a new single representation. In either case the method returns the appropriate
        object to work with.
        \param bInitialiseIfNeededNew If a new representation is needed, there is this additional option.
        When true (default) a normal copy of the existing (multiple referenced) representation is returned.
        When false, the new representation is not initialised and has undefined content. This is useful, when
        the whole matrix is intended to be replaced by something in the next step.*/
        MRep* get_own_copy(bool bInitialiseIfNeededNew = true)
        {
            if (iRefCount == 1)
                return this;
            --iRefCount;
            if (bInitialiseIfNeededNew)
                return new MRep(dim_rows, dim_cols, m);
            else
                return new MRep(dim_rows, dim_cols, 0);
        }

    private:
        // Frees all the memory used by the matrix representation.
        void free()
        {
            if (!m)
                return;
            for (uint32_t i = 0; i < dim_rows; ++i)
            {
                if (!m[i])
                    continue;
                delete[] m[i];
            }
            delete[] m;
            m = 0;
        }
    }; // MRep

    /* The internal representation of the matrix data. This could be shared by more than one matrix,
    if they are equal. The amount is reference counted.*/
    MRep* m_rep;
    /* Called by the destructor. Checks whether the representation is really no longer needed and
    can be deleted just like the containing matrix class can (because it's leaving its life scope).
    If there are other matrices with equal content out there, the representation nows about them by
    its counter and keeps being alive.*/
    void free()
    {
        if (--(m_rep->iRefCount) == 0 && m_rep)
        {
            delete m_rep;
            m_rep = 0;
        }
    }
    /* Changes a matrix entry. Of course the representation has to duplicate itself first, when more than
    one matrices share the representations content.*/
    void put(uint32_t row, uint32_t col, const T& t)
    {
        m_rep = m_rep->get_own_copy();
        m_rep->m[row][col] = t;
    }
}; // matrix

typedef matrix<float> matrixf;
typedef matrix<double> matrixd;
typedef matrix<int32_t> matrixi;

using std::complex;
//! Extends matrix<T> to cmatrix<t> which at its core is essentially the same as matrix<complex<t>>.
/** Inheritance seems natural, since this special choice of template type just results in a bunch
of additional possible operations on the matrix that are complex number related.
You could also use matrix<complex<t>> instead of cmatrix<t>. But then you would miss these operations.*/
template <class t>
class cmatrix : public matrix<complex<t>>
{
public:
    typedef complex<t> T; // for convenience

    // Constructors and assignment are transferred trivially.
    explicit cmatrix(uint32_t dim_rows, uint32_t dim_cols)
        : matrix<complex<t>>(dim_rows, dim_cols)
    {
    }
    cmatrix(uint32_t dim_rows, uint32_t dim_cols, T** mtrx)
        : matrix<complex<t>>(dim_rows, dim_cols, mtrx)
    {
    }
    cmatrix(const cmatrix& mtrx)
        : matrix<complex<t>>(mtrx)
    {
    }
    explicit cmatrix(const matrix<complex<t>>& mtrx)
        : matrix<complex<t>>(mtrx)
    {
    }
    cmatrix& operator=(const cmatrix& mtrx)
    {
        matrix<complex<t>>::operator=(mtrx);
        return *this;
    }
    ~cmatrix() = default;

    //! Conjugates this matrix.
    void conjugate()
    {
        matrix<complex<t>>::get_own_rep_copy();
        uint32_t r = matrix<complex<t>>::RowCount(), c = matrix<complex<t>>::ColCount();
        T** elem = *this;
        for (uint32_t i = 0; i < r; ++i)
            for (uint32_t j = 0; j < c; ++j)
            {
                elem[i][j] = std::conj(elem[i][j]);
            }
    }
    //! Adjoins this matrix. This method supports only quadratic matrices!
    void adjoin()
    {
        matrix<complex<t>>::get_own_rep_copy();
        uint32_t r = matrix<complex<t>>::RowCount();
        T** elem = *this;
        T aux;
        for (uint32_t i = 0; i < r; ++i)
        {
            for (uint32_t j = i + 1; j < r; ++j)
            {
                aux = std::conj(elem[i][j]);
                elem[i][j] = std::conj(elem[j][i]);
                elem[j][i] = aux;
            }
        }
        for (uint32_t i = 0; i < r; ++i)
            elem[i][i] = std::conj(elem[i][i]);
    }
    //! Is hermitean?
    [[nodiscard]] bool isHermitean() const
    {
        throw ul::not_implemented{"isHermitean"};
    }
    //! Is unitary?
    [[nodiscard]] bool isUnitary() const
    {
        throw ul::not_implemented{"isUnitary"};
    }
};

//! Sum. Both matrices have to coincide dimensionally.
template <typename T>
matrix<T> operator+(const matrix<T>& t1, const matrix<T>& t2)
{
    return matrix<T>(t1) += t2;
}
//! Difference. Both matrices have to coincide dimensionally.
template <typename T>
matrix<T> operator-(const matrix<T>& t1, const matrix<T>& t2)
{
    return matrix<T>(t1) -= t2;
}
//! Matrix multiplication.
/** Remark: Haven't yet found out why \code matrixd m3(0, 0); m3 = m1*m2; \endcode
is fast as usual, whereas \code matrixd m3(m1*m2); \endcode is two times slower. The sheer
existence of operator*= is not the reason (checked it).
No dimension check is performed, so pay attention that
\param t1 a mxk matrix (rows x columns) and
\param t2 is a kxn matrix,
to
\return a mxn matrix.*/
template <typename T>
matrix<T> operator*(const matrix<T>& t1, const matrix<T>& t2)
{
    return matrix<T>(t1) *= t2;
}
} // namespace mb::too::math

#endif
