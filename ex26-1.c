#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

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
        sleep(1);
        printf("child - parent pid is %d\n", getppid());
        exit(EXIT_SUCCESS);
    default:
        printf("parent - about to exit.\n");
        exit(EXIT_SUCCESS);

    }
}

