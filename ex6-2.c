#include <setjmp.h>
#include "tlpi_hdr.h"

static jmp_buf env;

static void
f2(void)
{
    longjmp(env, 1);
}


static void
f1(void)
{

    switch (setjmp(env)) {
    case 0:     /* This is the return after the initial setjmp() */
        printf("Calling f1() after initial setjmp()\n");
        break;                  /* ... but this is good form */

    case 1:
        printf("We jumped back from f1()\n");
        break;
    }
}

static void
f0(void)
{
    f1();
}

int
main(int argc, char *argv[])
{

    f0();

    printf("f1() has retured\n");
    f2();

    exit(EXIT_SUCCESS);
}
