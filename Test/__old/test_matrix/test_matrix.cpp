#include <tchar.h>
#include <cstdlib>
#include <iostream>
#include <conio.h>
#include <complex>
#include "Toolib/types.h"
#include "Toolib/math/matrix.h"
#include "Toolib/clock.h"
#include "Toolib/math/randomizer.h"
#include "Toolib/scope/raii_p.h"
//#define TOO_USE_VLD
#include "Toolib/debug.h"

TOOLOG_SET_LOG_FILENAME("!test_matrix.log");
TOOLOG_SET_LOG_TRACELEVEL(TL_ALL);

namespace
{
	using std::cout;
	using std::endl;
	using too::math::matrix;
	using too::math::matrixd;
	using too::math::matrixi;
	using too::math::randomizer;
	using too::math::cmatrix;
	using std::complex;
	typedef complex<int> complexi;
	typedef complex<double> complexd;

	template <class T> void matrix_out(const too::math::matrix<T>& m)
	{
		for (too::u32 i = 0; i < m.RowCount(); ++i)
		{
			for (too::u32 j = 0; j < m.ColCount(); ++j)
				cout << m(i, j) << " ";
			cout << endl;
		}
	}
	void naive_matrix_mult()
	{
		too::clock c;

		const too::u32 qsize = 500;
		too::raii_aap<double> pdm1(qsize, qsize);
		too::raii_aap<double> pdm2(qsize, qsize);
		double** dm1 = pdm1.cast();
		double** dm2 = pdm2.cast();
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = 1.0/randomizer::next(1, 100);
				dm2[i][j] = 1.0/randomizer::next(1, 100);
			}
		}
		cout << "Time needed to fill matrices in ms: " << c.elapsed() << endl;
		too::raii_aap<double> pdm3(qsize, qsize);
		double** dm3 = pdm3.cast();
		double dsum;
		double* dm1r;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dsum = 0.0;
				dm1r = dm1[i];
				for (int k = 0; k < qsize; ++k)
				{
					dsum+= dm1r[k]*dm2[k][j];
				}
				dm3[i][j] = dsum;
			}
		}
		cout << "Time needed to multiply matrices in ms: " << c.elapsed() << endl;
		system("PAUSE");
	}
	void matrix_perftest()
	{
		cout << "==================== matrix_perftest ===============================" << endl;
		cout << "Naive (fast) matrix multiplication..." << endl;
		cout << "=====================================" << endl;
		naive_matrix_mult();

		cout << "Matrix multiplication with Toolib..." << endl;
		cout << "====================================" << endl;

		too::clock c;

		const too::u32 qsize = 500;
		/*too::class_aaptr<double> pdm1(qsize, qsize);
		too::class_aaptr<double> pdm2(qsize, qsize);
		double** dm1 = pdm1.cast();
		double** dm2 = pdm2.cast();
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = 1.0/randomizer::next(1, 100);
				dm2[i][j] = 1.0/randomizer::next(1, 100);
			}
		}
		matrix<double> m1(qsize, qsize, dm1);
		matrix<double> m2(qsize, qsize, dm2);*/
		matrix<double> m1(qsize, qsize);
		matrix<double> m2(qsize, qsize);
		double** dm1 = m1;
		double** dm2 = m2;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = 1.0/randomizer::next(1, 100);
				dm2[i][j] = 1.0/randomizer::next(1, 100);
			}
		}
		/*cout << "m1 ==================================" << endl;
		matrix_out(m1);
		cout << "m2 ==================================" << endl;
		matrix_out(m2);
		//cout << "   ==================================" << endl;*/
		cout << "Time needed to fill matrices in ms: " << c.elapsed() << endl;
		//system("PAUSE");
		matrixd m3(0, 0);
		m3 = m1*m2;
		/*matrixd m3(0,0);
		m3 = m1*m2;*/
		cout << "Time needed to multiply matrices in ms: " << c.elapsed() << endl;
		/*cout << "m3 ==================================" << endl;
		matrix_out(m3);*/
		m3.zeroize();
		cout << "Time needed to zeroize matrix in ms: " << c.elapsed() << endl;
		system("PAUSE");
		/*m1 = m3;
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		cout << "m3 ==================================" << endl;
		matrix_out(m3);
		m3(1,1) = 0;
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		cout << "m3 ==================================" << endl;
		matrix_out(m3);*/
	}
	void matrix_test()
	{
		cout << "==================== matrix_test ===============================" << endl;
		const too::u32 qsize = 3;
		matrix<int> m1(qsize, qsize);
		matrix<int> m2(qsize, qsize);
		int** dm1 = m1;
		int** dm2 = m2;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = randomizer::next(0, 5);
				dm2[i][j] = randomizer::next(0, 5);
			}
		}
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		m1[0][2] = 1;
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		cout << m1[0][2] << endl;
		cout << "m2 ==================================" << endl;
		matrix_out(m2);
		matrix<int> m3(m1*m2);
		cout << "m1*m2 ===============================" << endl;
		matrix_out(m3);
		system("PAUSE");
	}
	void matrix_optest()
	{
		cout << "==================== matrix_optest ===============================" << endl;
		const too::u32 qsize = 3;
		matrix<int> m1(qsize, qsize);
		matrix<int> m2(qsize, qsize);
		int** dm1 = m1;
		int** dm2 = m2;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = randomizer::next(0, 5);
				dm2[i][j] = randomizer::next(0, 5);
			}
		}
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		cout << "m2 ==================================" << endl;
		matrix_out(m2);
		matrix<int> m3(qsize, qsize);
		m3 = m1+m2;
		cout << "m3=m1+m2 ==================================" << endl;
		matrix_out(m3);
		m3 = m1-m2;
		cout << "m3=m1-m2 ==================================" << endl;
		matrix_out(m3);
		matrix<int> m4(qsize, qsize);
		m4 = m3*(-1);
		cout << "m4=m3*(-1) ==================================" << endl;
		matrix_out(m4);
		m4 = 2*m3;
		cout << "m4=2*m3 ==================================" << endl;
		matrix_out(m4);
		m4/= 2;
		cout << "m4/=2 ==================================" << endl;
		matrix_out(m4);
		m4*= -1;
		cout << "m4*=-1 ==================================" << endl;
		matrix_out(m4);
		matrix<int> m4not0(qsize, qsize);
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				m4not0[i][j] = randomizer::next(1, 5);
				m4not0[i][j] = randomizer::next(1, 5);
			}
		}
		cout << "new random m4not0 ==================================" << endl;
		matrix_out(m4not0);
		matrix<int> m5(qsize, qsize);
		m5 = 1/m4not0;
		cout << "m5=1/m4not0 ==================================" << endl;
		matrix_out(m5);
		m5 = m4/2;
		cout << "m5=m4/2 ==================================" << endl;
		matrix_out(m5);
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		m5+=m1;
		cout << "m5+=m1 ==================================" << endl;
		matrix_out(m5);
		m5-=m1;
		cout << "m5-=m1 ==================================" << endl;
		matrix_out(m5);
		m5=m5*m1;
		cout << "m5*=m1 ==================================" << endl;
		matrix_out(m5);
		system("PAUSE");
	}
	void matrix_perftest2()
	{
		cout << "==================== matrix_perftest2 ===============================" << endl;
		too::clock c;
		const too::u32 qsize = 100;
		matrix<double> m1(qsize, qsize);
		matrix<double> m2(qsize, qsize);
		double** dm1 = m1;
		double** dm2 = m2;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = 1.0/randomizer::next(1, 100);
				dm2[i][j] = 1.0/randomizer::next(1, 100);
			}
		}
		cout << "Time needed to fill matrices in ms: " << c.elapsed() << endl;
		m1 = m1*m2;
		cout << "Time needed to multiply matrices with m1=m1*m2 in ms: " << c.elapsed() << endl;
		m1*= m2;
		cout << "Time needed to multiply matrices with m1*=m2 in ms: " << c.elapsed() << endl;
		system("PAUSE");
	}
	void matrix_comptest()
	{
		cout << "==================== matrix_comptest ===============================" << endl;
		too::clock c;
		const too::u32 qsize = 5000;
		matrix<double> m1(qsize, qsize);
		matrix<double> m2(qsize, qsize);
		double** dm1 = m1;
		double** dm2 = m2;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = 1.0;
				dm2[i][j] = 1.0;
			}
		}
		//dm2[2500][2500] = 0.5;
		cout << "Time needed to fill matrices in ms: " << c.elapsed() << endl;
		bool b = m1==m2;
		cout << "Time needed to compare matrices in ms: " << c.elapsed() << endl;
		cout << "Equal? " << b << endl;
		b = m1!=m2;
		cout << "Time needed to compare matrices in ms: " << c.elapsed() << endl;
		cout << "Not equal? " << b << endl;
		system("PAUSE");
	}
	void matrix_idtest()
	{
		cout << "==================== matrix_idtest ===============================" << endl;
		const too::u32 qsize = 10;
		matrix<int> m1(qsize, qsize);
		int** dm1 = m1;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = randomizer::next(0, 5);
			}
		}
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		bool bId = m1.identity();
		cout << "Id? " << bId << endl;
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		cout << "Make..." << endl;
		m1.identity(true);
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		bId = m1.identity();
		cout << "Id? " << bId << endl;
		system("PAUSE");
		cout << "==================== matrix_idtest performance ===============================" << endl;
		const too::u32 qsize2 = 5000;
		matrix<int> m2(qsize2, qsize2);
		too::clock c;
		m2.identity(true);
		cout << "Time making identity in ms: " << c.elapsed() << endl;
		bool bId2 = m2.identity();
		cout << "Time checking identity for identity in ms: " << c.elapsed() << endl;
		cout << "Id? " << bId2 << endl;
		system("PAUSE");
	}
	void matrix_colrowtranstest()
	{
		cout << "==================== matrix_colrowtranstest ===============================" << endl;
		const too::u32 qsize = 10;
		matrix<int> m1(qsize, qsize);
		int** dm1 = m1;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				dm1[i][j] = randomizer::next(0, 5);
			}
		}
		cout << "m1 ==================================" << endl;
		matrix_out(m1);
		matrix<int> mr = m1.getrow(5);
		matrix<int> mc = m1.getcol(2);
		cout << "mr(6) ==================================" << endl;
		matrix_out(mr);
		cout << "mc(3) ==================================" << endl;
		matrix_out(mc);
		m1.transpose();
		cout << "m1.transpose ==================================" << endl;
		matrix_out(m1);
		system("PAUSE");
		cout << "==================== matrix_colrowtranstest performance ====================" << endl;
		const too::u32 qsize2 = 1000;
		matrix<int> m2(qsize2, qsize2);
		too::clock c;
		for (int i = 0; i < 50; ++i)
			mr = m2.getrow(900+i);
		cout << "Time extracting row in ms: " << c.elapsed() << endl;
		for (int i = 0; i < 50; ++i)
			mc = m2.getcol(900+i);
		cout << "Time extracting col in ms: " << c.elapsed() << endl;
		m2.transpose();
		cout << "Time needed to transpose in ms: " << c.elapsed() << endl;
		for (int i = 0; i < 50; ++i)
			mc = m2.getrow(900+i);
		cout << "Time extracting \"col\" in ms: " << c.elapsed() << endl;
		system("PAUSE");
	}
	void matrix_cmatrixtest()
	{
		cout << "==================== matrix_cmatrixtest ===============================" << endl;
		cmatrix<int> cm1(2, 2);
		complexi** dm1 = cm1;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				dm1[i][j] = complexi(randomizer::next(-1, 1), randomizer::next(-1, 1));
			}
		}
		cout << "cm1 ==================================" << endl;
		matrix_out(cm1);
		cmatrix<int> cm2(2, 2);
		complexi** dm2 = cm2;
		for (int i = 0; i < 2; ++i)
		{
			for (int j = 0; j < 2; ++j)
			{
				dm2[i][j] = complexi(randomizer::next(-1, 1), randomizer::next(-1, 1));
			}
		}
		cout << "cm2 ==================================" << endl;
		matrix_out(cm2);
		cmatrix<int> cmres(0,0);
		cmres = cm1 * cm2;
		cout << "cmres=cm1*cm2 ==================================" << endl;
		matrix_out(cmres);
		cmres.conjugate();
		cout << "cmres.conj ==================================" << endl;
		matrix_out(cmres);
		cmres.adjoin();
		cout << "cmres.adj ==================================" << endl;
		matrix_out(cmres);
		system("PAUSE");
		cout << "==================== matrix_cmatrixtest performance ===============================" << endl;
		const too::u32 qsize = 100;
		cmatrix<double> m1(qsize, qsize);
		cmatrix<double> m2(qsize, qsize);
		complex<double>** ddm1 = m1;
		complex<double>** ddm2 = m2;
		for (int i = 0; i < qsize; ++i)
		{
			for (int j = 0; j < qsize; ++j)
			{
				ddm1[i][j] = complexd(1.0/randomizer::next(-4, 4), randomizer::next(-4, 4));
				ddm2[i][j] = complexd(1.0/randomizer::next(-4, 4), randomizer::next(-4, 4));
			}
		}
		too::clock c;
		cmatrix<double> m3(0, 0);
		m3 = m1*m2;
		cout << "Time for multiplying in ms: " << c.elapsed() << endl;
		m3.adjoin();
		cout << "Time for adjoin in ms: " << c.elapsed() << endl;
		m3.transpose();
		cout << "Time for transpose in ms: " << c.elapsed() << endl;
		system("PAUSE");
	}
	void matrix_misctest()
	{
		cout << "==================== matrix_misctest ===============================" << endl;
		matrixi m1(2, 2);
		matrixi m2(2, 2);
		m1 = m2;
		m1 = m2;
		m1 = m2;
		m1 = m2;
		cout << "m2  ==================================" << endl;
		matrix_out(m2);
		matrix<double> m3 = m2.matrix_cast<double>();
		cout << "m3  ==================================" << endl;
		matrix_out(m3);
		system("PAUSE");
	}
} // namespace
int _tmain(int argc, _TCHAR* argv[])
{
	matrix_test();
	matrix_perftest();
	matrix_optest();
	matrix_perftest2();
	matrix_comptest();
	matrix_idtest();
	matrix_colrowtranstest();
	matrix_cmatrixtest();
	matrix_misctest();

	TOO_DUMP_MEM_LEAKS;
	return 0;
}

