#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <pwd.h>

int
main(int argc, char *argv[])
{
	struct passwd *pp;

    printf("%ld %ld\n", (long) (getpwnam("avr")->pw_uid),
           (long) (getpwnam("tsr")->pw_uid));


	pp = getpwnam("avr");	
	printf("address of struct pointer is %p\n", (void *)pp);
	pp = getpwnam("tsr");	
	printf("address of struct pointer is %p\n", (void *)pp);
    printf("%ld\n", (long) (pp->pw_uid));

    exit(EXIT_SUCCESS);
}
