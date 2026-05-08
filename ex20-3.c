#define _GNU_SOURCE
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include "tlpi_hdr.h"

static volatile int round = 0;

static void
handler(int sig)
{
    int r;
    time_t now;

    r=++round;

    now = time(NULL);
    printf("%s Round %d: sighandler: signal %d was caught\n", ctime(&now), r, sig);

    sleep(5);

    now = time(NULL);
    printf("%s Round %d: sighandler: slept 5 seconds\n", ctime(&now), r);

    fflush(NULL);
}

int
main(int argc, char *argv[])
{
    int resethand, nodefer;
    struct sigaction sa;


    resethand = nodefer = 0;

    for (int i = 1; i < argc ; i++) {
        if(strcmp(argv[i], "SA_RESETHAND") == 0)
            resethand = 1;

        if(strcmp(argv[i], "SA_NODEFER") == 0)
            nodefer = 1;

    }

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;
    if (resethand)
        sa.sa_flags |= SA_RESETHAND;
    if (nodefer)
        sa.sa_flags |= SA_NODEFER;

    sa.sa_handler = handler;

    if (sigaction(SIGINT, &sa, NULL) == -1 )
        errExit("sigaction");

    while(1)
        pause();

    exit(EXIT_SUCCESS);
}
