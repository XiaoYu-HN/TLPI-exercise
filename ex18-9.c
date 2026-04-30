#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include "tlpi_hdr.h"

void chdir_method(void);
void fchdir_method(void);

void chdir_method()
{
    char buf[PATH_MAX];

    getcwd(buf, PATH_MAX);
    chdir("/");
    chdir(buf);
}


void fchdir_method()
{
    int fd;

    fd = open(".", O_RDONLY);
    chdir("/");
    fchdir(fd);
    close(fd);
}

int
main(int argc, char *argv[])
{

    int loops, i;
    clock_t before, after;

    if (argc != 2 || strcmp(argv[1], "--help") == 0)
        usageErr("%s loops\n", argv[0]);

    loops = atoi(argv[1]);
    if (loops == 0)
        loops = 1000;

    before = clock();
	for (i=0; i<loops; i++)
	    chdir_method();
    after = clock();
    printf("%s method:%ld cpu time\n", "chdir", after - before);


    before = clock();
	for (i=0; i<loops; i++)
    	fchdir_method();
    after = clock();
    printf("%s method:%ld cpu time\n", "fchdir", after - before);

    exit(EXIT_SUCCESS);
}
