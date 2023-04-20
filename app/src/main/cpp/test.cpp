#include "mcl_common.hpp"
#include <string>

std::string test_func(int a, int b)
{
	using namespace mcl::bn;
	Fp x = a;
	Fp y = b;
	std::string s = x.getStr() + "^" + y.getStr() + "=";
	Fp::pow(x, x, y);
	s += x.getStr();
	return s;
}
