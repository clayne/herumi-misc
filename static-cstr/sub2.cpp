#include <stdio.h>

#ifdef __GNUC__
	#define MCL_ATTRIBUTE __attribute__((constructor))
#else
	#define MCL_ATTRIBUTE
#endif

namespace {

static struct X1 {
	X1() { puts("sub2 X1 cstr"); }
} x1;

static void MCL_ATTRIBUTE initSub2()
{
	puts("initSub2");
}

#ifdef _MSC_VER
#pragma section(".CRT$XCT", read)
__declspec(allocate(".CRT$XCT")) void(*ptr_initSub2)() = initSub2;
#endif

static struct X2 {
	X2() { puts("sub2 X2 cstr"); }
} x2;


}
