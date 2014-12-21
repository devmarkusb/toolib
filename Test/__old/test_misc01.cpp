#include <tchar.h>
#include <iostream>
#include <conio.h>
#include <vector>
#include <string>
#include <malloc.h>
#include "Toolib/assert.h"
#include "Toolib/mem/checked_delete.h"
//#include "Toolib/class_ptr.h"
#include "Toolib/clock.h"
#include "Toolib/class/counted_class_objects.h"
#include "Toolib/GUI/keycodes.h"
#include "Toolib/mem/new_handler.h"
#include "Toolib/class/non_copyable.h"
#include "Toolib/PPDEFS.h"
#include "Toolib/scope/raii_p.h"
#include "Toolib/math/randomizer.h"
#include "Toolib/scope/scopeguard.h"
#include "Toolib/types.h"
#include "Toolib/math/vector.h"
//#define TOO_USE_VLD
#include "Toolib/debug.h"

TOOLOG_SET_LOG_FILENAME("!test_misc01.log");
TOOLOG_SET_LOG_TRACELEVEL(TL_ALL);

namespace
{
	using std::cout;
	using std::endl;

	void clock_test()
	{
		cout << "=============== clock_test =================" << endl;
		/*clock_t t = too::clock::gettime();
		cout << t << endl;
		for (; t <= 2000; t = too::clock::gettime());
		t = too::clock::gettime();
		cout << t << endl;*/
		too::clock c;
		cout << c.elapsed(too::clock::NANOSEC, false) << " (now: " << c.now(false) << ")" << endl;
		_getch();
		cout << c.elapsed(too::clock::MICROSEC, false) << endl;
		c.SpeedFactor() = 5.0;
		_getch();
		cout << c.elapsed(too::clock::SEC) << endl;
		//cout << clock() << endl;
	}
	void rand_test()
	{
		cout << "=============== rand_test =================" << endl;
		using too::math::randomizer;
		for (int i = 1; i <= 10; ++i)
		{
			cout << randomizer::next(0, 1) << " ";
		}
		cout << endl;
		for (int i = 1; i <= 10; ++i)
		{
			cout << randomizer::next() << " ";
		}
		cout << endl;
		for (int i = 1; i <= 10; ++i)
		{
			cout << randomizer::next(5, 10) << " ";
		}
		cout << endl;
		for (int i = 1; i <= 10; ++i)
		{
			cout << randomizer::next(100000, 999999) << " ";
		}
		cout << endl;
	}
	void ptr_test()
	{
		cout << "=============== ptr_test =================" << endl;
		using too::raii_p;

		class C1
		{
		public:
			C1(){}
			void f(){}
		};

		C1 c01;
		C1* pc22 = new C1();
		raii_p<C1> c1;
		//c1 = &c01;
		raii_p<C1> c11(new C1());
		raii_p<C1> c22(pc22);
		//class_ptr<C1> c222(pc22); // possible, but bad idea, pc22 gets deleted twice
		//class_ptr<C1> c111(c11);
		//class_ptr<C1> c111(&c01);
		//c1 = c11; //ok
		//c1 = &c01; //?
		//delete c11; //?
		C1* pc1;
		//pc1 = 0;
		//c1 = new C1(); //?
		c1->f();
		raii_p<C1>* pcp1 = new raii_p<C1>(new C1());
		pcp1->reset(new C1());
		//pcp1->reset(c1);
		delete pcp1;
		*c1 = *c11;
		pc1 = c1.release();
		c1.reset(pc1);
		//delete pc1;
		pc1 = 0;

		using too::math::randomizer;
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
	}
	void rand_test2()
	{
		cout << "=============== rand_test2 =================" << endl;
		using too::math::randomizer;
		double r;
		bool sgn;
		int help;
		cout << "RAND_MAX: " << RAND_MAX << endl;
		for (int i = 0; i < 10; ++i)
		{
			r = randomizer::next(0,9);
			r+= randomizer::next(0,9)/10.0;
			r+= randomizer::next(0,9)/100.0;
            help = randomizer::next(1,100);
			cout << help << "....";
			sgn = help%2==0;
			if (!sgn)
				r*= -1.0;
			cout << r << endl;
		}
		cout << "Using lib ftcs..." << endl;
		for (int i = 0; i < 10; ++i)
		{
			r = randomizer::next_sign()*randomizer::next(0,100,3);
			cout << r << endl;
		}
	}
	void sg_test()
	{
		cout << "=============== sg_test =================" << endl;
		std::vector<std::string> svec;
		svec.push_back("example string");
		too::ScopeGuard guardSVPush = too::MakeObjGuard(svec, &std::vector<std::string>::pop_back);

		// ... do other stuff ...

		guardSVPush.NoRollback();

		void* buffer = std::malloc(1024);
		too::ScopeGuard sgFreeIt = too::MakeGuard(std::free, buffer);

		// no NoRollback for sgFreeIt (it's not a rollback, it's a clean up always to be performed)
	}
	class X : public too::NewHandlerSupport<X>
	{
	public:
		static void OutOfMemory()
		{
			cout << endl << "ERROR: Out of memory!" << endl;
			system("PAUSE");
			//throw std::bad_alloc();
		}
	};
	void new_handler_test()
	{
		cout << "=============== new_handler_test =================" << endl;
		X::set_new_handler(X::OutOfMemory);
		//X* x = new X[1000000000];
		//delete[] x;
	}
} // namespace
int _tmain(int argc, _TCHAR* argv[])
{
	using std::cout;
	using std::endl;

	//TOO_DEBUG_BREAK_IF(2!=2);
	//int* pi = new int;
	/*rand_test();
	clock_test();
	ptr_test();
	rand_test2();
	try{
	sg_test();
	}
	catch(...){}
	new_handler_test();*/

	system("PAUSE");

	TOO_DUMP_MEM_LEAKS;
	return 0;
}

