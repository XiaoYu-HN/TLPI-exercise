#include <signal.h>
#include <sys/wait.h>
#include "curr_time.h"                  /* Declaration of currTime() */
#include "tlpi_hdr.h"

#define SYNC_SIG SIGCHLD                /* Synchronization signal */

static void             /* Signal handler - does nothing but return */
handler(int sig)
{
    printf("[%s %ld] In signal handler.\n", currTime("%T"), (long) getpid());
}

int
main(int argc, char *argv[])
{
    pid_t childPid;
    sigset_t blockMask;
    struct sigaction sa;
	int status;

    setbuf(stdout, NULL);               /* Disable buffering of stdout */

    sigemptyset(&blockMask);
    sigaddset(&blockMask, SYNC_SIG);    /* Block signal */
    if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
        errExit("sigprocmask");

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sa.sa_handler = handler;
    if (sigaction(SYNC_SIG, &sa, NULL) == -1)
        errExit("sigaction");

    switch (childPid = fork()) {
    case -1:
        errExit("fork");

    case 0: /* Child */


        printf("[%s %ld] Child started - doing some work\n",
               currTime("%T"), (long) getpid());

		sleep(2);
        /* And then signals parent that it's done */

        printf("[%s %ld] Child about to signal parent\n",
               currTime("%T"), (long) getpid());
        if (kill(getppid(), SYNC_SIG) == -1)
            errExit("kill");

        /* Now child can do other things... */

        _exit(EXIT_SUCCESS);

    default: /* Parent */
        if (sigprocmask(SIG_UNBLOCK, &blockMask, NULL) == -1)
            errExit("sigprocmask");
		if(sleep(5))
        printf("[%s %ld] Parent sleep() is interrupted\n",
               currTime("%T"), (long) getpid());
			
        printf("[%s %ld] Parent about to wait for signal\n",
               currTime("%T"), (long) getpid());
        childPid = waitpid(childPid, &status, 0);
        if (childPid == -1)
            errExit("waitpid");
        else
            printf("[%s %ld] Parent wait for child %d (status = %d)succeed\n", currTime("%T"), (long) getpid(), childPid, status);


        exit(EXIT_SUCCESS);
    }
}
