#define _GNU_SOURCE
#include <signal.h>
#include "tlpi_hdr.h"

void my_abort(void);

void my_abort(void)
{

    struct sigaction sa;
    sigset_t unblockingMask;

    // unblocks the SIGABRT signal
    sigemptyset(&unblockingMask);        /* Unblock all signals */
    sigaddset(&unblockingMask, SIGABRT);
    if (sigprocmask(SIG_UNBLOCK, &unblockingMask, NULL) == -1)
        perror("sigprocmask");

    // raise SIGABRT signal
    raise(SIGABRT);

	// If execution reaches here
	// it means the user-defined signal handler has returned (without calling exit or longjmp).
	//
    // restoring the default disposition for SIGABRT
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = SIG_DFL;
    if (sigaction(SIGABRT, &sa, NULL) == -1 )
        perror("sigaction");

    // raise SIGABRT signal for a second time
    raise(SIGABRT);
}


int
main(int argc, char *argv[])
{

    my_abort();

    exit(EXIT_SUCCESS);
}
