#include <tchar.h>
#include <cstdlib>
#include <iostream>
#include "Toolib/math/vector.h"
#include "Toolib/math/randomizer.h"
#include "Toolib/clock.h"
#include "Toolib/debug.h"

TOOLOG_SET_LOG_FILENAME("!test_vector.log");
TOOLOG_SET_LOG_TRACELEVEL(TL_ALL);

namespace
{
	using std::cout;
	using std::endl;
	using too::math::vector;
	using too::math::vector2d;
	using too::math::vector3d;
	using too::math::vector2dd;
	using too::math::vector3dd;
	using too::u32;
	using too::math::randomizer;
	using too::clock;

	template <class T, u32 DIM> void vec_out(vector<T, DIM> v)
	{
		for (u32 i = 0; i < DIM; ++i)
			cout << v.x[i] << " ";
		cout << endl;
	}
	void test_vec_constr()
	{
		cout << "=========== test_vec_constr ===============" << endl;
		int* ai = new int[2];
		ai[0] = 0; ai[1] = 0;
		vector<int, 2> i2v(ai);
		delete[] ai;
		vector<int, 2> i2v2(i2v);
		//i2v2 = i2v;
		i2v.x[0] = 5;
		vec_out(i2v2);
		vec_out(i2v);
		//vector<int, 1000000> ilargev(true);
		vector<double, 2> d2v = i2v;//.vector_cast<double>();
		cout << "d2v ======" << endl;
		vec_out(d2v);
		d2v.x[0] = 2.6;
		cout << "d2v ======" << endl;
		vec_out(d2v);
		i2v = d2v;//.vector_cast<int>(); // makes a cast warning
		cout << "i2v ======" << endl;
		vec_out(i2v);
		system("PAUSE");
	}
	void test_vec_misc()
	{
		cout << "=========== test_vec_misc ===============" << endl;
		vector2d<double> x;
		vector2d<double> z(0.5, 0.25);
		vector2d<double> zc = z;
		vector2d<double> zc2(z);
		cout << "x  =====" << endl;
		vec_out(x);
		cout << "z  =====" << endl;
		vec_out(z);
		cout << "zc =====" << endl;
		vec_out(zc);
		cout << "zc2=====" << endl;
		vec_out(zc2);
		x.set(0.5, -0.5);
		cout << "x =====" << endl;
		vec_out(x);
		cout << "x length: " << x.length() << endl;
		cout << "z length: "  << z.length() << endl;
		cout << "z length^2: "  << z.lengthsq() << endl;

		vector3dd a(-1, 0, 2);
		vector3dd b(1, -1, 1);
		cout << "a =====" << endl;
		vec_out(a);
		cout << "b =====" << endl;
		vec_out(b);
		vector3dd c = b*2.0;
		cout << "c=b*2 =====" << endl;
		vec_out(c);
		a=2*b;
		cout << "a=2*b =====" << endl;
		vec_out(a);
		double t = dot_product(b, c);
		cout << "t=b*c =====" << endl;
		cout << t << endl;
		a = b+c;
		cout << "a=b+c =====" << endl;
		vec_out(a);
		a = b-c;
		cout << "a=b-c =====" << endl;
		vec_out(a);
		cout << "getDIM a: " << a.getDIM() << endl;
		cout << "a[0]: " << a[0] << "a[1]: " << a[1] << "a[2]: " << a[2] << endl;
		cout << ".x ----- a[0]: " << a.x[0] << "a[1]: " << a.x[1] << "a[2]: " << a.x[2] << endl;
		a = c/2;
		cout << "a=c/2 =====" << endl;
		vec_out(a);
		float fdummy[3] = {2,4,4};
		vector<float, 3> d(fdummy);
		cout << "d =====" << endl;
		vec_out(d);
		a = 2/d;//.vector_cast<double>();
		cout << "a=2/d =====" << endl;
		vec_out(a);
		a*= 2;
		cout << "a*=2 =====" << endl;
		vec_out(a);
		a/= 2;
		cout << "a/=2 =====" << endl;
		vec_out(a);
		a+= d;//.vector_cast<double>();
		cout << "a+=d =====" << endl;
		vec_out(a);
		a-= d;//.vector_cast<double>();
		cout << "a-=d =====" << endl;
		vec_out(a);
		cout << "b =====" << endl;
		vec_out(b);
		cout << "c =====" << endl;
		vec_out(c);
		a = c-b*0.5;
		cout << "a=c-b*0.5 =====" << endl;
		vec_out(a);
		system("PAUSE");
	}
	void test_vec_perf()
	{
		cout << "=========== test_vec_perf ===============" << endl;
		const int C_iloops = 10000;
		vector3dd v, vto, op; 
		vector<double, 3> v_, v_to, op_;
		for (int i = 0; i < 3; ++i)
		{
			v.x[i] = 1.0*randomizer::next(1, 10)-randomizer::next(0, 10);
			v_.x[i] = 1.0*randomizer::next(1, 10)-randomizer::next(0, 10);
			op.x[i] = 1.0*randomizer::next(1, 10)-randomizer::next(0, 10);
			op_.x[i] = 1.0*randomizer::next(1, 10)-randomizer::next(0, 10);
		}
		cout << "v =====" << endl;
		vec_out(v);
		cout << "v_ =====" << endl;
		vec_out(v_);
		cout << "op =====" << endl;
		vec_out(op);
		cout << "op_ =====" << endl;
		vec_out(op_);
		clock c;
		for (int i = 0; i < C_iloops; ++i)
		{
			vto+= 0.01*v;
		}
		cout << "Time for assignemnts vector3d in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			vector3dd vtoc(0.01*i*v);
		}
		cout << "Time for copy constructions vector3d in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			v_to+= 0.01*v_;
		}
		cout << "Time for assignemnts vector<,> in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			vector<double, 3> v_toc(0.01*i*v_);
		}
		cout << "Time for copy constructions vector<,> in ms: " << c.elapsed() << endl;
		double t=0.0;
		for (int i = 0; i < C_iloops; ++i)
		{
			t+= 0.01*dot_product(v, op);
		}
		cout << "Time for dot products vector3d in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			t+= 0.01*dot_product(v_, op_);
		}
		cout << "Time for dot products vector<,> in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			v[2]+= 0.01*t;
		}
		cout << "Time for index access v[] vector3d in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			v_[2]+= 0.01*t;
		}
		cout << "Time for index access v[] vector<,> in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			v.x[2]+= 0.01*t;
		}
		cout << "Time for index access v.x[] vector3d in ms: " << c.elapsed() << endl;
		for (int i = 0; i < C_iloops; ++i)
		{
			v_.x[2]+= 0.01*t;
		}
		cout << "Time for index access v.x[] vector<,> in ms: " << c.elapsed() << endl;
		double *pt = v.x;
		for (int i = 0; i < C_iloops; ++i)
		{
			*(pt+2)+= 0.01*t;
		}
		cout << "Time for index access via pointer vector3d in ms: " << c.elapsed() << endl;
		pt = v_.x;
		for (int i = 0; i < C_iloops; ++i)
		{
			*(pt+2)+= 0.01*t;
		}
		cout << "Time for index access via pointer vector<,> in ms: " << c.elapsed() << endl;

		cout << "v =====" << endl;
		vec_out(v);
		cout << "v_ =====" << endl;
		vec_out(v_);
		cout << "op =====" << endl;
		vec_out(op);
		cout << "op_ =====" << endl;
		vec_out(op_);
		v = v_;
		op_ = op;
		cout << "v=v_ =====" << endl;
		vec_out(v);
		cout << "op_=op =====" << endl;
		vec_out(op_);
		vector3dd w(v_); 
		vector<double, 3> w_(op);
		cout << "constr w(v_) =====" << endl;
		vec_out(w);
		cout << "constr w_(op) =====" << endl;
		vec_out(w_);
		system("PAUSE");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	test_vec_constr();
	test_vec_misc();
	test_vec_perf();

	TOO_DUMP_MEM_LEAKS;
	return 0;
}

