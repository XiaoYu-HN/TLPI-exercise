#define _GNU_SOURCE
#include <signal.h>
#include "signal_functions.h"           /* Declaration of printSigset() */
#include "tlpi_hdr.h"

static int sigCnt = 0;
static volatile sig_atomic_t gotSigint = 0;

static void
handler(int sig)
{
    printf("Signal %d was caught\n", sig);

    if (sig == SIGINT)
        gotSigint = 1;
    else
        sigCnt++;
}

int
main(int argc, char *argv[])
{
    int numSecs;
    sigset_t pendingMask, blockingMask, emptyMask;
    struct sigaction sa;

    printf("%s: PID is %ld\n", argv[0], (long) getpid());

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGUSR1, &sa, NULL) == -1 )
        errExit("sigaction");

    if (sigaction(SIGINT, &sa, NULL) == -1 )
        errExit("sigaction");


    if (argc > 1) {
        numSecs = getInt(argv[1], GN_GT_0, NULL);

        sigfillset(&blockingMask);
        if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
            errExit("sigprocmask");

        printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
        sleep(numSecs);

        if (sigpending(&pendingMask) == -1)
            errExit("sigpending");

        printf("%s: pending signals are: \n", argv[0]);
        printSigset(stdout, "\t\t", &pendingMask);

        if (argc > 2) {
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sa.sa_handler = SIG_IGN;
            if (sigaction(SIGUSR1, &sa, NULL) == -1 )
                errExit("sigaction");
        }

        sigemptyset(&emptyMask);        /* Unblock all signals */
        if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
            errExit("sigprocmask");
    }

    while (!gotSigint)                  /* Loop until SIGINT caught */
        continue;

    fprintf(stderr, "%s: signal SIGUSR1 caught %d time%s\n", argv[0],
            sigCnt, (sigCnt == 1) ? "" : "s");

    exit(EXIT_SUCCESS);
}
