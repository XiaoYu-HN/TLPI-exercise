#include <signal.h>
#include <time.h>
#include "tlpi_hdr.h"

static void
handler(int sig)
{
}

#define TESTSIG SIGUSR1

int
main(int argc, char *argv[])
{
    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s num-sigs\n", argv[0]);

    int numSigs = getInt(argv[1], GN_GT_0, "num-sigs");

    struct sigaction sa;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = handler;
    if (sigaction(TESTSIG, &sa, NULL) == -1)
        errExit("sigaction");

    /* Block the signal before fork(), so that the child doesn't manage
       to send it to the parent before the parent is ready to catch it */

    sigset_t blockedMask, emptyMask, *pendingMask;
    sigemptyset(&blockedMask);
    sigaddset(&blockedMask, TESTSIG);
    if (sigprocmask(SIG_SETMASK, &blockedMask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&emptyMask);

    time_t timeBegin, timeEnd;
    pendingMask = &blockedMask;
    pid_t childPid = fork();
    switch (childPid) {
    case -1:
        errExit("fork");

    case 0:     /* child */
        timeBegin = time(NULL);
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (kill(getppid(), TESTSIG) == -1)
                errExit("kill");
            if (sigwaitinfo(pendingMask, NULL) == -1 && errno != EINTR)
                errExit("sigsuspend");
        }
        timeEnd = time(NULL);
        printf("Child process: elapsed time is %ld seconds\n", timeEnd - timeBegin);
        exit(EXIT_SUCCESS);

    default: /* parent */
        timeBegin = time(NULL);
        for (int scnt = 0; scnt < numSigs; scnt++) {
            if (sigwaitinfo(pendingMask, NULL) == -1 && errno != EINTR)
                errExit("sigsuspend");
            if (kill(childPid, TESTSIG) == -1)
                errExit("kill");
        }
        timeEnd = time(NULL);
        printf("Parent process: elapsed time is %ld seconds\n", timeEnd - timeBegin);
        exit(EXIT_SUCCESS);
    }
}
