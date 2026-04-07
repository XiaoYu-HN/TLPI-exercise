#include <fcntl.h>
#include "tlpi_hdr.h"


int dup(int oldfd);
int dup2(int oldfd, int newfd);

int dup(int oldfd)
{
	if (fcntl(oldfd, F_GETFL) == -1) {
		errno = EBADF;
		return -1;
	}

	return fcntl(oldfd, F_DUPFD, 0);

}

int dup2(int oldfd, int newfd)
{
	int fd;

	if (fcntl(oldfd, F_GETFL) == -1) {
		errno = EBADF;
		return -1;
	}

	if (oldfd == newfd)
		fd = newfd;

	close(newfd);
	fd = fcntl(oldfd, F_DUPFD, newfd);

	return fd;

}

int
main(int argc, char *argv[])
{

    exit(EXIT_SUCCESS);
}
