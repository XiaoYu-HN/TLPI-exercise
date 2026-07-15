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
	int wait_status;


    pid = fork();
    switch (pid) {
    case -1:
        perror("fork");
        exit(EXIT_FAILURE);
    case 0:
        puts("child - About to exit(-1).");
		exit(-1);
    default:
        printf("parent - About to wait.");
		wait(&wait_status);
		printf("child exit status is %d\n", wait_status);
		printf("child exit status(WEXITSTATUS) is %d\n", WEXITSTATUS(wait_status));
		printf("child exit status(WEXITSTATUS) is %d\n", (((wait_status) & 0xff00) >> 8));
    }
}

