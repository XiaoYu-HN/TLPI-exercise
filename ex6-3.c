#include <stdlib.h>
#include <string.h>
#include "tlpi_hdr.h"


int my_unsetenv(const char *name);
int my_setenv(const char *name, const char *value, int overwrite);

int my_unsetenv(const char *name){
	if (name == NULL || *name == '\0' || strchr(name, '=') != NULL) {
		errno = EINVAL;
		return -1;
	}

	char *n = getenv(name);	

	if ( n == NULL) return 0;

	return putenv(name);
}

int my_setenv(const char *name, const char *value, int overwrite){
	char *buf;

	if (name == NULL || *name == '\0' || strchr(name, '=') != NULL) {
		errno = EINVAL;
		return -1;
	}

	char *n = getenv(name);

	if (n == NULL || overwrite) {
		errno = 0;
		buf = malloc(strlen(name) + 1 + strlen(value));
		if (errno != 0) return -1;
		sprintf("%s=%s", name, value);
		putenv(buf);
		free(buf);
	} 

	return 0;
}

int
main(int argc, char *argv[])
{

    exit(EXIT_SUCCESS);
}
