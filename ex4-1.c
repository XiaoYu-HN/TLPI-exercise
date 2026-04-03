#include "tlpi_hdr.h"
#include <stdbool.h>
#include <fcntl.h>

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

static bool teeAppend = false;
static bool haveFiles = false;

int
main(int argc, char *argv[])
{
    int opt;
    int fd, openFlags;
    mode_t filePerms;
    ssize_t numRead;
    char buf[BUF_SIZE];

    while ((opt = getopt(argc, argv, "a")) != -1) {
        switch (opt) {
        case 'a':
            teeAppend = true;
            break;
        default:
            usageErr("%s [-a] filename...\n", argv[0]);
        }
    }

    if (optind >= argc) {
        printf("tee to stdout, append is %s\n", teeAppend ? "true":"false");
    } else {
        haveFiles = true;
        printf("tee to below files, append is %s\n", teeAppend ? "true":"false");
        for (int i = optind; i < argc; i++) {
            printf("  [%d] %s\n", i - optind + 1, argv[i]);
        }

    }

    if (teeAppend)
        openFlags = O_CREAT | O_WRONLY | O_APPEND;
    else
        openFlags = O_CREAT | O_WRONLY | O_TRUNC;

    filePerms = 0644;

    fd = open(argv[optind], openFlags, filePerms);

    if (fd == -1)
        errExit("opening file %s", argv[2]);

    while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, numRead) != numRead)
            fatal("write() to stdout returned error or partial write occurred");

        if (write(fd, buf, numRead) != numRead)
            fatal("write() to file returned error or partial write occurred");
    }

    if (numRead == -1)
        errExit("read");

    if (close(fd) == -1)
        errExit("close file");

    exit(EXIT_SUCCESS);
}
