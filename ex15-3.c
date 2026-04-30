#define _GNU_SOURCE
#include <fcntl.h>           /* Definition of AT_* constants */
#include <sys/stat.h>
#include <sys/sysmacros.h>
#include <time.h>
#include "file_perms.h"
#include "tlpi_hdr.h"

#define MAX_STR_SIZE 1024

void  printtime_with_nsec(char *prefix, const struct statx_timestamp *st);

static void
displayStatInfo(const struct statx *sb)
{
    printf("File type:                ");

    switch (sb->stx_mode & S_IFMT) {
    case S_IFREG:
        printf("regular file\n");
        break;
    case S_IFDIR:
        printf("directory\n");
        break;
    case S_IFCHR:
        printf("character device\n");
        break;
    case S_IFBLK:
        printf("block device\n");
        break;
    case S_IFLNK:
        printf("symbolic (soft) link\n");
        break;
    case S_IFIFO:
        printf("FIFO or pipe\n");
        break;
    case S_IFSOCK:
        printf("socket\n");
        break;
    default:
        printf("unknown file type?\n");
        break;
    }

    printf("Device containing i-node: major=%ld   minor=%ld\n",
           (long) sb->stx_dev_major, (long) sb->stx_dev_minor);

    printf("I-node number:            %ld\n", (long) sb->stx_ino);

    printf("Mode:                     %lo (%s)\n",
           (unsigned long) sb->stx_mode, filePermStr(sb->stx_mode, 0));

    if (sb->stx_mode & (S_ISUID | S_ISGID | S_ISVTX))
        printf("    special bits set:     %s%s%s\n",
               (sb->stx_mode & S_ISUID) ? "set-UID " : "",
               (sb->stx_mode & S_ISGID) ? "set-GID " : "",
               (sb->stx_mode & S_ISVTX) ? "sticky " : "");

    printf("Number of (hard) links:   %ld\n", (long) sb->stx_nlink);

    printf("Ownership:                UID=%ld   GID=%ld\n",
           (long) sb->stx_uid, (long) sb->stx_gid);

    if (S_ISCHR(sb->stx_mode) || S_ISBLK(sb->stx_mode))
        printf("Device number (st_rdev):  major=%ld; minor=%ld\n",
               (long) sb->stx_rdev_major, (long) sb->stx_rdev_minor);

    printf("File size:                %lld bytes\n", (long long) sb->stx_size);
    printf("Optimal I/O block size:   %ld bytes\n", (long) sb->stx_blksize);
    printf("512B blocks allocated:    %lld\n", (long long) sb->stx_blocks);

    printtime_with_nsec("Last file access:         ", &sb->stx_atime);
    printtime_with_nsec("Last file modification:   ", &sb->stx_mtime);
    printtime_with_nsec("Last status change:       ", &sb->stx_ctime);
}


void printtime_with_nsec(char *prefix, const struct statx_timestamp *st) {
    struct tm *tm;
    char fmt[MAX_STR_SIZE];
    char s[MAX_STR_SIZE];
	char nsec[MAX_STR_SIZE];

    tm = localtime((time_t *)&st->tv_sec);

	snprintf(nsec, sizeof(nsec), "%.3f", (double)st->tv_nsec/1e9);
    snprintf(fmt, sizeof(fmt), "%s: %%a %%b %%d %%H:%%M:%%S%s %%G", prefix, strchr(nsec, '.'));

    strftime(s, sizeof(s), fmt, tm);
    printf("%s\n", s);

}

int
main(int argc, char *argv[])
{
    struct statx sb;
    Boolean statLink;           /* True if "-l" specified (i.e., use lstat) */
    int fname;                  /* Location of filename argument in argv[] */
    int flags = 0;

    statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
    /* Simple parsing for "-l" */
    fname = statLink ? 2 : 1;

    if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
        usageErr("%s [-l] file\n"
                 "        -l = use lstat() instead of stat()\n", argv[0]);

    if (statLink == FALSE)
        flags |= AT_SYMLINK_NOFOLLOW;

    if (statx(AT_FDCWD, argv[fname], flags, STATX_ALL, &sb) == -1)
        errExit("statx");

    displayStatInfo(&sb);

    exit(EXIT_SUCCESS);
}
