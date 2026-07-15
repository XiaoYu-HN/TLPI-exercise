#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define TESTSIG SIGABRT
int
main(void)
{
    pid_t pid;


    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        puts("child - Child exiting.");
        raise(TESTSIG);
        puts("child - Never get here.");
		_exit(0);
    default:
        printf("parent - Child is PID %jd\n", (intmax_t) pid);
        for (;;)
            pause();
        puts("parent - Never get here.");
    }
}

