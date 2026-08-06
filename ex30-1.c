#include <pthread.h>
#include "tlpi_hdr.h"

static volatile int glob = 0;   /* "volatile" prevents compiler optimizations
                                   of arithmetic operations on 'glob' */

typedef struct {
	int tid;
	int loops;
} Thread_args;

static void *                   /* Loop arg->loops times incrementing 'glob' */
threadFunc(void *arg)
{
	Thread_args* targ = (Thread_args *) arg;
    int loc, j;

    for (j = 0; j < targ->loops; j++) {
        loc = glob;
        loc++;
        glob = loc;
		printf("t%d:%d\n", targ->tid, glob);
    }

    return NULL;
}

int
main(int argc, char *argv[])
{
    pthread_t t1, t2;
	Thread_args targ1, targ2;
    int s;

	targ1.loops = targ2.loops = (argc > 1) ? getInt(argv[1], GN_GT_0, "num-loops") : 10000000;
	targ1.tid = 1;
	targ2.tid = 2;

    s = pthread_create(&t1, NULL, threadFunc, &targ1);
    if (s != 0)
        errExitEN(s, "pthread_create");
    s = pthread_create(&t2, NULL, threadFunc, &targ2);
    if (s != 0)
        errExitEN(s, "pthread_create");

    s = pthread_join(t1, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");
    s = pthread_join(t2, NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    printf("glob = %d\n", glob);
    exit(EXIT_SUCCESS);
}
