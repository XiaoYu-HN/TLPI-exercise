#include <signal.h>
#include <libgen.h>             /* For basename() declaration */
#include "tlpi_hdr.h"

#define CMD_SIZE 200
#define SYNC_SIG SIGUSR1

static void             /* Signal handler - does nothing but return */
handler(int sig)
{
}

int
main(int argc, char *argv[])
{
    char cmd[CMD_SIZE];
    pid_t childPid;
	struct sigaction sa;

    setbuf(stdout, NULL);       /* Disable buffering of stdout */

    printf("Parent PID=%ld\n", (long) getpid());

    switch (childPid = fork()) {
    case -1:
        errExit("fork");

    case 0:     /* Child: immediately exits to become zombie */
		sleep(2);
        printf("Child (PID=%ld) exiting, notify parent by signal.\n", (long) getpid());
		kill(getppid(), SYNC_SIG);
        _exit(EXIT_SUCCESS);

    default:    /* Parent */
		sigemptyset(&sa.sa_mask);
		sa.sa_flags = SA_RESTART;
		sa.sa_handler = handler;
		if (sigaction(SYNC_SIG, &sa, NULL) == -1)
            errExit("sigaction");
		
		puts("Wait signal from child.");	
		pause();
        snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
        system(cmd);            /* View zombie child */

        /* Now send the "sure kill" signal to the zombie */

        if (kill(childPid, SIGKILL) == -1)
            errMsg("kill");
        sleep(3);               /* Give child a chance to react to signal */
        printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
        system(cmd);            /* View zombie child again */

        exit(EXIT_SUCCESS);
    }
}
