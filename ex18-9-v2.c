#include <fcntl.h>
#include <time.h>
#include <limits.h>
#include "tlpi_hdr.h"

static int fd;
static char buf[PATH_MAX];

void chdir_method(void);
void fchdir_method(void);

void chdir_method()
{
    chdir("/");
    chdir(buf);
}


void fchdir_method()
{

    chdir("/");
    fchdir(fd);
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

    getcwd(buf, PATH_MAX);
    before = clock();
	for (i=0; i<loops; i++)
	    chdir_method();
    after = clock();
    printf("%s method:%ld cpu time\n", "chdir", after - before);


    fd = open(".", O_RDONLY);
    before = clock();
	for (i=0; i<loops; i++)
    	fchdir_method();
    after = clock();
    close(fd);
    printf("%s method:%ld cpu time\n", "fchdir", after - before);

    exit(EXIT_SUCCESS);
}
