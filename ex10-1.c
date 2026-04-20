#include <stdio.h>
#include <unistd.h>
#include <sys/times.h>
#include <time.h>
#include <stdint.h>
#include "tlpi_hdr.h"


int
main(int argc, char *argv[])
{

    clock_t t, c, pt;
    uint32_t max = UINT32_MAX;
    struct tms buf;
    long  jiff = sysconf(_SC_CLK_TCK);

    printf("size of clock_t is %ld\n", sizeof(clock_t));
    printf("max unsigned int value is %u\n", UINT32_MAX);

    printf("_SC_CLK_TCK is %ld\n", jiff);
    printf("CLOCKS_PER_SEC is %ld\n", CLOCKS_PER_SEC);

    t = times(&buf);
    if (t == -1)
        errExit("times");

    c = clock();

    pt = buf.tms_utime + buf.tms_stime + buf.tms_cutime + buf.tms_cstime;
    printf("processor time by times() is %ld _SC_CLK_TCK (%ld seconds)\n", pt, pt/jiff);
    printf("for times() overflow, %ld seconds left\n", (max - pt)/jiff);

    printf("processor time by clock() is %ld CLOCKS_PER_SEC (%ld seconds)\n", c, c/CLOCKS_PER_SEC);
    printf("for clock() overflow, %ld seconds left\n", (max - c)/CLOCKS_PER_SEC);


    exit(EXIT_SUCCESS);

}

