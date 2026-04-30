#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include "tlpi_hdr.h"


int
main(int argc, char *argv[])
{

    char *pathname;
    bool isdir, anyread;
    struct stat sb;
    mode_t m;


    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s pathname\n", argv[0]);

    pathname = argv[1];
    if (stat(pathname, &sb) == -1)
        errExit("stat");

    m = sb.st_mode;
    isdir = S_ISDIR(m) ? true : false;
    anyread = ((m & S_IXUSR) == S_IXUSR) || ((m & S_IXGRP) == S_IXGRP) || ((m & S_IXOTH) == S_IXOTH);

    if (isdir || anyread) {
        m |= S_IRUSR | S_IRGRP | S_IROTH;
        m |= S_IXUSR | S_IXGRP | S_IXOTH;
    }


    if (chmod(pathname, m) == -1)
        errExit("chmod");

    exit(EXIT_SUCCESS);
}
