#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <grp.h>
#include "tlpi_hdr.h"


int
main(int argc, char *argv[])
{

    gid_t grouplist[NGROUPS_MAX + 1];
    int ngrps;


    printf("calling process gid is:%d\n", getgid());
    printf("calling process egid is:%d\n", getegid());

    ngrps = getgroups(NGROUPS_MAX, grouplist);
    if (ngrps == -1)
        errExit("getgroups");

    for(int i = 0; i < ngrps; i++)
        printf("calling process supplement gid is:%d\n", grouplist[i]);

    exit(EXIT_SUCCESS);
}

