#include <pthread.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int s;

    printf("Message from main()\n");
    s = pthread_join(pthread_self(), NULL);
    if (s != 0)
        errExitEN(s, "pthread_join");

    exit(EXIT_SUCCESS);
}
