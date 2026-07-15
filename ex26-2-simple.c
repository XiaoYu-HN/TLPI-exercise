#include <signal.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include "tlpi_hdr.h"

void second_fork(void);

void second_fork(void)
{
    pid_t pid, ppid, adopt_ppid;

    pid = fork();

    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
		ppid = getppid();
		while((adopt_ppid = getppid()) == ppid){
			sleep(1);
		}
        printf("child is adopted by pid %d.\n", adopt_ppid);
        exit(EXIT_SUCCESS);
    default:
        printf("parent - about to sleep(2).\n");
		sleep(2);
        printf("parent - about to exit.\n");
        exit(EXIT_SUCCESS);

    }
}

int
main(void)
{
    pid_t pid ;

    pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        second_fork();
        exit(EXIT_SUCCESS);
    default:
        printf("grandparent - about to sleep(5).\n");
        sleep(5);
        printf("grandparent - about to wait.\n");
		wait(NULL);
        exit(EXIT_SUCCESS);

    }
}

