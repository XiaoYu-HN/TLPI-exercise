#include <sys/wait.h>
#include <time.h>
#include "tlpi_hdr.h"

#define LOOPS 100000
int
main(int argc, char *argv[])
{
    int status;
    pid_t childPid;
    clock_t start, end;
    struct timespec e_start, e_end;
    double elapsed, cpu_time_used;

    start = clock();
    clock_gettime(CLOCK_MONOTONIC, &e_start);

    for (int i = 0; i < LOOPS; i++) {

        switch (argc > 1 ? vfork() : fork()) {
        case -1:
            errExit("fork");

        case 0:
            _exit(EXIT_SUCCESS);

        default:
            childPid = waitpid(-1, &status, 0);
            if (childPid == -1)
                errExit("waitpid");

        }
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    clock_gettime(CLOCK_MONOTONIC, &e_end);

    elapsed = (e_end.tv_sec - e_start.tv_sec) +
              (e_end.tv_nsec - e_start.tv_nsec) / 1e9;

    printf("Elapsed time(CPU time) is: %f(%f) seconds\n", elapsed, cpu_time_used);
}
