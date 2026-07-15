#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

#define SYNC_SIG SIGUSR1

static void             /* Signal handler - does nothing but return */
handler(int sig)
{
}

void second_fork(const int *pipefd);

void second_fork(const int *pipefd)
{
    struct sigaction sa;
    pid_t child_pid;

    child_pid = fork();

    switch (child_pid) {
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
        close(pipefd[0]);
        write(pipefd[1], &child_pid, sizeof(pid_t));
        close(pipefd[1]);
        printf("parent - about to exit.\n");
        exit(EXIT_SUCCESS);

    }
}

int
main(void)
{
    pid_t pid, child_to_kill;
    int pipefd[2];

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        second_fork(pipefd);
        exit(EXIT_SUCCESS);
    default:
        sleep(5);
        printf("grandparent - about to wait.\n");
        wait(NULL);
        close(pipefd[1]);
        read(pipefd[0], &child_to_kill, sizeof(pid_t));
        close(pipefd[0]);
        kill(child_to_kill, SYNC_SIG);
        exit(EXIT_SUCCESS);

    }
}

