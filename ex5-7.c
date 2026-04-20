#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

struct iovec {
    void   *iov_base;  /* Starting address */
    size_t  iov_len;   /* Size of the memory pointed to by iov_base. */
};

ssize_t readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t writev(int fd, const struct iovec *iov, int iovcnt);



ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{

    size_t s = 0, count, left;
	char *readbuf, *p;

    if (iovcnt <= 0) {
        errno = EINVAL;
        return -1;
    }

    for (int i = 0; i < iovcnt; i++) {
        if (iov[i].iov_len <= 0) {
            errno = EINVAL;
            return -2;
        }
        s += iov[i].iov_len;
    }

	readbuf = malloc(s);
	if (readbuf == NULL)
		return -3;

	
	count = read(fd, readbuf, s);
	if (count == -1)
		return -1;

	p = readbuf;	
	left = count;
    for (int i = 0; i < iovcnt; i++) {
		if (left > iov[i].iov_len) {	
			strncpy(iov[i].iov_base, p, iov[i].iov_len);
			left -= iov[i].iov_len;
		} else {
			strncpy(iov[i].iov_base, p, left);
			break;
		}
        p += iov[i].iov_len;
    }

	return count;
}


ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{

    size_t s = 0;
	char *writebuf, *p;

    if (iovcnt <= 0) {
        errno = EINVAL;
        return -1;
    }

    for (int i = 0; i < iovcnt; i++) {
        if (iov[i].iov_len <= 0) {
            errno = EINVAL;
            return -2;
        }
        s += iov[i].iov_len;
    }

	writebuf = malloc(s);
	if (writebuf == NULL)
		return -3;

	
	p = writebuf;	
    for (int i = 0; i < iovcnt; i++) {
		strncpy(p, iov[i].iov_base, iov[i].iov_len);
        p += iov[i].iov_len;
    }

	return write(fd, writebuf, s);
}
