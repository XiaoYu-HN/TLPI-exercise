#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <sys/types.h>
#include <grp.h>
#include "tlpi_hdr.h"


int my_initgroups(const char *user, gid_t group);

int my_initgroups(const char *user, gid_t group)
{
    gid_t grouplist[NGROUPS_MAX + 1];
	int size;
    struct group *grp;
    char  **p;

	size = 1;
	grouplist[0] = group;
    while ((grp = getgrent()) != NULL) {

        p = grp->gr_mem;
        while (*p != NULL) {
            if (strcmp(user, *p) == 0) {
                printf("user %s is a member of group %s\n", user, grp->gr_name);
				grouplist[size++] = grp->gr_gid;
                break;
            }
            p++;
        }

    }
    endgrent();

	if (setgroups(size, grouplist) == -1)
		errExit("setgroups");		

    return 0;
}

int
main(int argc, char *argv[])
{

    int gid;

    if (argc != 3)
        usageErr("%s username gid\n", argv[0]);

    gid = getInt(argv[2], GN_GT_0, "gid");

    my_initgroups(argv[1], gid);
    exit(EXIT_SUCCESS);
}

