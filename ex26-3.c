#include <sys/wait.h>
#include "tlpi_hdr.h"

void print_wait_status(siginfo_t *infop);

void print_wait_status(siginfo_t *infop)
{

    int status = infop->si_status;
    int code = infop->si_code;

    if (code == CLD_EXITED) {
        printf("child exited, status=%d\n", WEXITSTATUS(status));

    } else if (code == CLD_KILLED || code == CLD_DUMPED) {
        printf("child killed by signal %d (%s)",
               status, strsignal(status));
        if (code == CLD_DUMPED)
            printf(" (core dumped)");
        printf("\n");

    } else if (code == CLD_STOPPED) {
        printf("child stopped by signal %d (%s)\n",
               status, strsignal(status));

    } else if (code == CLD_CONTINUED) {
        printf("child continued\n");
    } else {            /* Should never happen */
        printf("what happened to this child? (status=%x)\n",
               (unsigned int) status);
    }
}

int
main(int argc, char *argv[])
{
    siginfo_t infop;

    if (argc > 1 && strcmp(argv[1], "--help") == 0)
        usageErr("%s [exit-status]\n", argv[0]);

    switch (fork()) {
    case -1:
        errExit("fork");

    case 0:             /* Child: either exits immediately with given
                           status or loops waiting for signals */
        printf("Child started with PID = %ld\n", (long) getpid());
        if (argc > 1)                   /* Status supplied on command line? */
            exit(getInt(argv[1], 0, "exit-status"));
        else                            /* Otherwise, wait for signals */
            for (;;)
                pause();
        exit(EXIT_FAILURE);             /* Not reached, but good practice */

    default:            /* Parent: repeatedly wait on child until it
                           either exits or is terminated by a signal */
        for (;;) {
            if(waitid(P_ALL, 0, &infop, WEXITED|WSTOPPED|WCONTINUED) == -1)
                errExit("waitid");

            printf("waitpid() returned: PID=%ld; status=0x%04x (%d,%d)\n",
                   (long) infop.si_pid,
                   infop.si_status, infop.si_status >> 8, infop.si_status & 0xff);

            print_wait_status(&infop);

            int code = infop.si_code;
            if(code == CLD_EXITED || code == CLD_KILLED || code == CLD_DUMPED)
                exit(EXIT_SUCCESS);
        }
    }
}
