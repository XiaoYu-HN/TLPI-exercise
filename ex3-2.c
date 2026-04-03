#include "tlpi_hdr.h"

void getInt_test(const char *src, int flags, const char *msg);


int main()
{
	char *s = "20";

	getInt_test(s, 0, "base 10");

	getInt_test(s, GN_BASE_8, "base 8");

	getInt_test(s, GN_BASE_16, "base 16");

	s = "0x20";
	getInt_test(s, GN_ANY_BASE, "base any");

	s = "020";
	getInt(s, GN_ANY_BASE, "base any");

	// failure case
	s = "abc";
	getInt_test(s, 0, "base 10");

	s = "-20";
	getInt(s, GN_NONNEG, "non negative");


	exit(EXIT_SUCCESS);
}

void getInt_test(const char *src, int flags, const char *msg) {
	int a;
	a=getInt(src, flags, msg);
	printf("source is %s, %s result is %d\n", src, msg, a);

}
