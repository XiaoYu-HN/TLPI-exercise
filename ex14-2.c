#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <limits.h>
#include "tlpi_hdr.h"

bool is_valid_dir(char *dirname);

bool is_valid_dir(char *dirname)
{
    struct stat sb;
    if (stat(dirname, &sb) == -1)
        return false;

    return S_ISDIR(sb.st_mode) ? true : false;
}


int
main(int argc, char *argv[])
{

	char fname[PATH_MAX + 1];
    int nof;
    int fd;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s number_of_files directory\n", argv[0]);

    if (is_valid_dir(argv[2]) == false)
        errExit("%s is not a directory!\n", argv[2]);

    nof = getInt(argv[1], GN_GT_0, "number_of_files");

    for (int i = 0; i < nof; i++) {

        sprintf(fname, "%s/x%06d", argv[2], i);


        fd = open(fname, O_CREAT | O_WRONLY | O_EXCL, 0600);
        if (fd == -1)
            errExit("open");

        if (write(fd, "x", 1) == -1)
            errExit("write");

        close(fd);
    }


    for (int i = 0; i < nof; i++) {
        sprintf(fname, "%s/x%06d", argv[2], i);

        if (remove(fname) == -1)
            errExit("remove");
    }

    exit(EXIT_SUCCESS);
}
