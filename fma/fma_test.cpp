#include <cybozu/benchmark.hpp>

#include "fma.h"

const int N = 5*7*8*9*1000;

template<int n>
void bench()
{
	int C = 100;
	CYBOZU_BENCH_C("", 100, fmaN<n>, N);
    printf("n=%d %f\n", n, cybozu::bench::g_clk.getClock()/double(N)/C);
}
int main()
{
	bench<1>();
	bench<2>();
	bench<3>();
	bench<4>();
	bench<5>();
	bench<6>();
	bench<7>();
	bench<8>();
	bench<9>();
	bench<10>();
}


