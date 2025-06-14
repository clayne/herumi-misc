#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#ifndef _MSC_VER
#define CYBOZU_BENCH_USE_GETTIMEOFDAY
#endif
#include <cybozu/benchmark.hpp>
#include <cybozu/option.hpp>
#include "constdiv.hpp"

int g_mode;

extern "C" {

uint32_t div7org(uint32_t x);
uint32_t div7org2(uint32_t x);
uint32_t div7a(uint32_t x);

uint32_t div7b(uint32_t x);

} // extern "C"

typedef uint32_t (*DivFunc)(uint32_t);

uint64_t loop1(DivFunc f, uint32_t n)
{
	uint64_t sum = 0;
	for (uint32_t x = 0; x < n; x++) {
		sum += f(x);
	}
	return sum;
}

uint64_t loop2(DivFunc f, uint32_t n)
{
	uint32_t x = 123;
	for (uint32_t i = 0; i < n; i++) {
		x = x * f(x) + 12345;
	}
	return x;
}

void loopTest(const char *msg, uint64_t (*loop)(DivFunc f, uint32_t n), DivFunc gen)
{
	const uint32_t n = 10000000;
	const int C = 100;

	uint64_t r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0;
	puts(msg);
	CYBOZU_BENCH_C("org", C, r0 += loop, div7org, n);
	CYBOZU_BENCH_C("org2", C, r1 += loop, div7org2, n);
	CYBOZU_BENCH_C("a  ", C, r2 += loop, div7a, n);
	CYBOZU_BENCH_C("b  ", C, r3 += loop, div7b, n);
	if (gen) {
		CYBOZU_BENCH_C("gen", C, r4 += loop, gen, n);
	}
	CYBOZU_BENCH_C("org", C, r0 += loop, div7org, n);
	CYBOZU_BENCH_C("org2", C, r1 += loop, div7org2, n);
	CYBOZU_BENCH_C("a  ", C, r2 += loop, div7a, n);
	CYBOZU_BENCH_C("b  ", C, r3 += loop, div7b, n);
	if (gen) {
		CYBOZU_BENCH_C("gen", C, r4 += loop, gen, n);
	}
	printf("sum=%" PRIx64 " %" PRIx64 " %" PRIx64 " %" PRIx64 " %" PRIx64 "\n", r0, r1, r2, r3, r4);
	if ((r0 ^ r1) | (r0 ^ r2) | (r0 ^ r3) | (gen && (r0 ^ r4))) {
		puts("ERR");
	} else {
		puts("ok");
	}
}

int main(int argc, char *argv[])
{
	cybozu::Option opt;
	opt.appendOpt(&g_mode, 0, "m", "mode");
	opt.appendHelp("h");
	if (opt.parse(argc, argv)) {
		opt.put();
	} else {
		opt.usage();
	}
	DivFunc gen = 0;

#ifdef CONST_DIV_GEN
	ConstDivGen cdg;
	cdg.init(7);
	cdg.dump();
	gen = cdg.divd;
#endif
	loopTest("loop1", loop1, gen);
	loopTest("loop2", loop2, gen);
#if 0
	#pragma omp parallel for
	for (uint64_t x_ = 0; x_ <= 0xffffffff; x_++) {
		uint32_t x = uint32_t(x_);
		uint32_t o = div7org(x);
		uint32_t a = div7a(x);
		uint32_t b = div7b(x);
		if (o != a || o != b) {
			printf("ERR x=%u o=%u a=%u b=%u\n", x, o, a, b);
		}
	}
	puts("ok");
#endif
}
