#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <dirent.h>
#include "tlpi_hdr.h"
#include "ugid_functions.h"

/*
 * We need below fields:
 *
 * Name:   vim
 * Pid:    16582
 * Uid:    1000    1000    1000    1000
 * Uid:    RUID    EUID    SUID    FSUID
 *
 */

#define STR_MAX_LEN 1024

int process_pid(int uid, pid_t pid);

int process_pid(int uid, pid_t pid)
{

    char pathname[PATH_MAX];
    char buf[STR_MAX_LEN + 1];
    char cmdname[STR_MAX_LEN + 1] = "";
    FILE *fp;
    int finduser = 0;
    int findcmd = 0;

    sprintf(pathname, "/proc/%d/status", pid);
	fp = fopen(pathname, "r");
    if (fp == NULL)
        return -1;


    while (fgets(buf, 1024, fp) != NULL) {
        if (strncmp("Name:", buf, 5) == 0)	{
            strcpy(cmdname, buf + 5);
            findcmd=1;
        }

        if (strncmp("Uid:", buf, 4) == 0)
            if (strtol(buf + 4, NULL, 10) == uid)
                finduser = 1;
    }

    if (finduser && findcmd)
        printf("Pid: %d, Command: %s\n", pid, cmdname);

    fclose(fp);

    return 0;
}

int
main(int argc, char *argv[])
{

    uid_t uid;
    char *username;
    DIR *dirp;
    struct dirent *dp;
    pid_t pid;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s username\n", argv[0]);

    username = argv[1];
    uid = userIdFromName(username);
    if((int)uid == -1)
        errExit("userIdFromName");

    printf("uid of user %s is %d\n", username, uid);


    dirp = opendir("/proc");
    if (dirp == NULL)
        errExit("opendir");

    while ((dp = readdir(dirp)) != NULL) {
        if (dp->d_name[0] >= '0' && dp->d_name[0] <= '9') {
            pid = strtol(dp->d_name, NULL, 10);


            process_pid(uid, pid);
        }
    }

    closedir(dirp);
    exit(EXIT_SUCCESS);
}
