#include <sys/stat.h>
#include <fcntl.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int fd;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s\n", argv[0]);

    fd = open(argv[1],  O_CREAT | O_WRONLY | O_TRUNC |O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
        errExit("open");

    
    if (write(fd, "12345", 5) == -1)
        errExit("write");

    if (lseek(fd, 0, SEEK_SET) == -1)
        errExit("lseek");

    if (write(fd, "67890", 5) == -1)
        errExit("write");

    close(fd);

    exit(EXIT_SUCCESS);
}
