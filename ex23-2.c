#define _POSIX_C_SOURCE 199309L
#include <sys/time.h>
#include <time.h>
#include <signal.h>
#include "tlpi_hdr.h"

static void
sigintHandler(int sig)
{
    return;                     /* Just interrupt nanosleep() */
}

int
main(int argc, char *argv[])
{
    struct timespec start, finish, request;
    struct sigaction sa;
    int s;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s secs nanosecs\n", argv[0]);

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigintHandler;
    if (sigaction(SIGINT, &sa, NULL) == -1)
        errExit("sigaction");

    if(clock_gettime(CLOCK_REALTIME, &start) == -1)
        errExit("clock_gettime");

    request.tv_sec = getLong(argv[1], 0, "secs") + start.tv_sec;
    request.tv_nsec = getLong(argv[2], 0, "nanosecs") + start.tv_nsec;


    for (;;) {
        s = clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, &request, NULL);
        if (s == -1 && errno != EINTR)
            errExit("clock_nanosleep");

        if(clock_gettime(CLOCK_REALTIME, &finish) == -1)
            errExit("clock_gettime");
        printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec +
               (finish.tv_nsec - start.tv_nsec) / 100000000.0);

        if (s == 0)
            break;

        printf("Remaining: %9.6f secs\n", request.tv_sec - finish.tv_sec +
               (request.tv_nsec - finish.tv_nsec) / 100000000.0);
    }

    printf("Sleep complete\n");
    exit(EXIT_SUCCESS);
}
