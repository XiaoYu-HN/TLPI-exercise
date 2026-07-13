#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"           /* Declaration of printSigset() */
#include "tlpi_hdr.h"


static void
handler(int sig)
{
    //printf("Sighandler: signal %d was caught!\n", sig);
    printf("%d, ", sig);
    fflush(NULL);
}

int
main(int argc, char *argv[])
{
    int n;
    sigset_t blockingMask, emptyMask;
    struct sigaction sa;


    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    for (n = 1; n < NSIG; n++) {
		// if error, ignore and continue
        if (sigaction(n, &sa, NULL) == -1 )
            errMsg("sigaction for signal %d", n);
    }

    sigfillset(&blockingMask);
    if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
        errExit("sigprocmask");

    for (n = 1; n < NSIG; n++) {
        if (n != SIGKILL && n != SIGSTOP)
            raise(n);
    }

    printf("press Enter to continue...\n");
    getchar();
	
	printf("The order of signals recieved:\n");
    fflush(NULL);

/*
 * 	sigset_t pendingMask;
    if (sigpending(&pendingMask) == -1)
        errExit("sigpending");

    printf("%s: pending signals are: \n", argv[0]);
    printSigset(stdout, "\t\t", &pendingMask);
*/
    sigemptyset(&emptyMask);        /* Unblock all signals */
    if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
        errExit("sigprocmask");


    printf("\n");
    exit(EXIT_SUCCESS);
}
