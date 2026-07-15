#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

volatile sig_atomic_t child_pid;

#define SYNC_SIG SIGUSR1

static void             /* Signal handler - does nothing but return */
handler(int sig)
{
}

static void second_fork(void)
{
    struct sigaction sa;

    switch (child_pid = fork()) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:

        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_RESTART;
        sa.sa_handler = handler;
        if (sigaction(SYNC_SIG, &sa, NULL) == -1)
            errExit("sigaction");

        sleep(2);
        printf("ppid is %d when parent exit.\n", getppid());
        pause();
        printf("ppid is %d when grandparent wait.\n", getppid());
        exit(EXIT_SUCCESS);
    default:
        printf("parent - about to exit.\n");
        exit(EXIT_SUCCESS);

    }
}

int
main(void)
{
    pid_t pid;

    pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        second_fork();
        exit(EXIT_SUCCESS);
    default:
        sleep(5);
        printf("grandparent - about to wait.\n");
        wait(NULL);
        kill(child_pid, SYNC_SIG);
        exit(EXIT_SUCCESS);

    }
}

