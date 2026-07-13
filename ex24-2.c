#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int
main(void)
{
    pid_t pid;

    if (signal(SIGCHLD, SIG_IGN) == SIG_ERR) {
        perror("signal");
        exit(EXIT_FAILURE);
    }
    pid = vfork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        puts("child - Child exiting.");
		close(STDOUT_FILENO);
        exit(EXIT_SUCCESS);
    default:
        printf("parent - Child is PID %jd\n", (intmax_t) pid);
        puts("parent - Parent exiting.");
        exit(EXIT_SUCCESS);
    }
}

