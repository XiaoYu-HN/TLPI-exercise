#define _GNU_SOURCE
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "tlpi_hdr.h"

char *my_getcwd(char *buf, size_t size);
ino_t get_curdir_ino(void);

ino_t get_curdir_ino(void)
{
    struct stat sb;

    if(stat(".", &sb) == -1) {
        perror("stat");
        return (ino_t)-1;
    }
    return sb.st_ino;
}

char *my_getcwd(char *buf, size_t size)
{
    ino_t ino_cur, parent_ino_cur;
    int is_root = 0;
    char parent_dir[PATH_MAX] = "..";
    char tmpstr[PATH_MAX];
    DIR *dirp;
    struct dirent *dp;

    buf[0] = '\0';
    ino_cur = get_curdir_ino();

    //printf("ino is %ld\n", ino_cur);

    while (!is_root) {
        dirp = opendir(parent_dir);

        if (dirp  == NULL) {
            errMsg("opendir failed on '%s'", parent_dir);
            return NULL;
        }


        for (;;) {
            errno = 0;              /* To distinguish error from end-of-directory */
            dp = readdir(dirp);
            if (dp == NULL)
                break;

            if (strcmp(dp->d_name, "..") == 0) {
                if (dp->d_ino == ino_cur) {
                    is_root = 1;
                }

                continue;
            }

            if (strcmp(dp->d_name, ".") == 0) {
                parent_ino_cur = dp->d_ino;
                continue;
            }

            if(dp->d_ino == ino_cur) {
                if (buf[0] == '\0') {
                    strcpy(buf, dp->d_name);
                } else {
                    strcpy(tmpstr, buf);
                    strcpy(buf, dp->d_name);
                    strcat(buf, "/");
                    strcat(buf, tmpstr);
                }

                ino_cur = parent_ino_cur;

                strcat(parent_dir, "/..");
                //        	printf("%s\n", buf);

                break;
            }
        }

        if (errno != 0)
            errExit("readdir");

        if (closedir(dirp) == -1)
            errMsg("closedir");

    }


    strcpy(tmpstr, buf);
    strcpy(buf, "/");
    strcat(buf, tmpstr);

    return buf;
}


int
main(int argc, char *argv[])
{
    int fd_cwd;
    char cwd[PATH_MAX];

    if (argc !=1)
        usageErr("%s\n", argv[0]);

    printf("official CWD is %s\n", get_current_dir_name());

    fd_cwd = open(".", O_DIRECTORY|O_PATH);
    if (fd_cwd == -1)
        errExit("open");

    my_getcwd(cwd, PATH_MAX);

    printf("my version CWD is %s\n", cwd);

    fchdir(fd_cwd);
    close(fd_cwd);

    exit(EXIT_SUCCESS);
}
