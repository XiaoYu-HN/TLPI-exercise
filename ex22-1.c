#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "tlpi_hdr.h"

static void
handler(int sig)
{
    printf("Sighandler: signal %d was caught!\n", sig);
    fflush(NULL);
}

int
main(int argc, char *argv[])
{
    sigset_t blockingMask, pendingMask;
    struct sigaction sa;

    sigemptyset(&blockingMask);
    sigaddset(&blockingMask, SIGCONT);
    if (sigprocmask(SIG_BLOCK, &blockingMask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(SIGCONT, &sa, NULL) == -1 )
        errExit("sigaction");

    printf("Stopping process with SIGTSTP...\n");
    fflush(stdout);
    raise(SIGTSTP);

    printf("Process resumed after SIGCONT\n");
    printf("Check: is the handler invoked?\n");

    sigpending(&pendingMask);
    if (sigismember(&pendingMask, SIGCONT))
        printf("signal SIGCONT is pending.\n");
    else
        printf("signal SIGCONT is NOT pending.\n");


    printf("Press Enter to unblock SIGCONT...\n");
    getchar();

    if (sigprocmask(SIG_UNBLOCK, &blockingMask, NULL) == -1)
        errExit("sigprocmask");


    sigpending(&pendingMask);
    if (sigismember(&pendingMask, SIGCONT))
        printf("signal SIGCONT is pending.\n");
    else
        printf("signal SIGCONT is NOT pending.\n");

    exit(EXIT_SUCCESS);
}
