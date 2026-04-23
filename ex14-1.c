#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <limits.h>
#include "tlpi_hdr.h"

bool is_valid_dir(char *dirname);

static int
cmpintp(const void *p1, const void *p2)
{
    int a = *(const int *) p1;
    int b = *(const int *) p2;

    if (a == b) return 0;

    return (a < b) ? -1:1;

}

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
    int nof, r, notdup;
    int *fnlist;
    int fd;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s number_of_files directory\n", argv[0]);

    if (is_valid_dir(argv[2]) == false)
        errExit("%s is not a directory!\n", argv[2]);

    nof = getInt(argv[1], GN_GT_0, "number_of_files");
    fnlist = calloc(nof, sizeof(int));

    for (int i = 0; i < nof; i++) {
        do {
            notdup = true;
            // make r greater than 0
            r = 1 + rand() % 999999;
            for (int j=0; j<i; j++) {
                if (r == fnlist[j]) {
                    notdup = false;
                    //printf("Find dup:%d\n", r);
                    break;
                }
            }
        }
        while (notdup == false);

        sprintf(fname, "%s/x%06d", argv[2], r);
        //printf("filename is %s\n", fname);
        fnlist[i] = r;


        fd = open(fname, O_CREAT | O_WRONLY | O_EXCL, 0600);
        if (fd == -1)
            errExit("open");
        if (write(fd, "x", 1) == -1)
            errExit("write");
        close(fd);
    }

    qsort(fnlist, nof, sizeof(int), cmpintp);

    for (int i = 0; i < nof; i++) {
        sprintf(fname, "%s/x%06d", argv[2], fnlist[i]);
        //printf("remove filename is %s\n", fname);
        if (remove(fname) == -1)
            errExit("remove");
    }

    exit(EXIT_SUCCESS);
}
