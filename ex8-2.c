#include <pwd.h>
#include <string.h>
#include "tlpi_hdr.h"

struct passwd *my_getpwnam(const char *name);

struct passwd *my_getpwnam(const char *name)
{
    struct passwd *pwd;

    while ((pwd = getpwent()) != NULL) {
        if (strcmp(name, pwd->pw_name) == 0)
            return pwd;
    }

    endpwent();

    return NULL;
}


int
main(int argc, char *argv[])
{
    if (argc != 2)
        usageErr("%s username\n", argv[0]);

    if (my_getpwnam(argv[1]))
        printf("user %s is found in passwd file.\n", argv[1]);
    else
        printf("user %s is NOT found in passwd file.\n", argv[1]);


    exit(EXIT_SUCCESS);
}
