#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd, flags, loop_times;
    char buf[] = "*";
    bool append = false;

    if (argc < 3 || argc > 4 || strcmp(argv[1], "--help") == 0)
        usageErr("%s\n", argv[0]);

    flags = O_CREAT | O_WRONLY;
    if (argc == 3) {
        flags |= O_APPEND;
        append = true;
    }

    fd = open(argv[1], flags, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    loop_times = getInt(argv[2], GN_GT_0, "loop_times");

    for (int i = 0; i < loop_times; i++) {
        if (append == false) {
            if (lseek(fd, 0, SEEK_END) == -1)
                errExit("lseek");
        }

        if (write(fd, buf, 1) == -1)
            errExit("write");
    }


    close(fd);

    exit(EXIT_SUCCESS);
}
