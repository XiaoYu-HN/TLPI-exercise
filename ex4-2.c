#define _GNU_SOURCE
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"

#ifndef BUF_SIZE        /* Allow "cc -D" to override definition */
#define BUF_SIZE 1024
#endif

int
main(int argc, char *argv[])
{
    int fd_in, fd_out, openFlags;
    mode_t filePerms;
    off_t cur, hole_start, data_start, data_end, file_end;
    ssize_t len, bytes_copied;

    if (argc != 3 || strcmp(argv[1], "--help") == 0)
        usageErr("%s old-file new-file\n", argv[0]);

    /* Open input and output files */

    fd_in = open(argv[1], O_RDONLY);
    if (fd_in == -1)
        errExit("opening file %s", argv[1]);

    openFlags = O_CREAT | O_WRONLY | O_TRUNC;
    filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
                S_IROTH | S_IWOTH;      /* rw-rw-rw- */
    fd_out = open(argv[2], openFlags, filePerms);
    if (fd_out == -1)
        errExit("opening file %s", argv[2]);

    /* Transfer data until we encounter end of input or an error */

    hole_start=cur=data_start=data_end=0;
    file_end=lseek(fd_in, 0, SEEK_END);
    while (hole_start != file_end) {
        data_start=lseek(fd_in, hole_start, SEEK_DATA);

        // there is no data left, just hole
        if (data_start == -1 && errno == ENXIO)
            break;

        hole_start=lseek(fd_in, data_start, SEEK_HOLE);

        data_end = hole_start -1;

        printf("Copy data from %ld to %ld\n", data_start, data_end);

        lseek(fd_in, data_start, SEEK_SET);
        lseek(fd_out, data_start, SEEK_SET);

        len = data_end - data_start + 1;
        bytes_copied = 0;

        while (bytes_copied != len) {
            bytes_copied += copy_file_range(fd_in, NULL, fd_out, NULL, len, 0);
        }

    }


    if (close(fd_in) == -1)
        errExit("close input");
    if (close(fd_out) == -1)
        errExit("close output");

    exit(EXIT_SUCCESS);
}
