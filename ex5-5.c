#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"


int
main(int argc, char *argv[])
{
	int oldfd, newfd;
	off_t old_offset, new_offset;

	if (argc != 2) 
            usageErr("%s filename\n", argv[0]);
    

	oldfd = open(argv[1], O_RDONLY);
	if (oldfd == -1)
		errExit("open");

	newfd = dup(oldfd);		
	if (newfd == -1)
		errExit("dup");
		

	old_offset = lseek(oldfd, 0, SEEK_END);		
	if (old_offset == -1)
		errExit("lseek");


	new_offset = lseek(newfd, 0, SEEK_CUR);		
	if (new_offset == -1)
		errExit("lseek");

	if (new_offset == old_offset) 
		printf("file offset is the same: %ld\n", old_offset);
	else
		printf("file offset is NOT the same: %ld vs %ld\n", old_offset, new_offset);
		
	close(oldfd);
	close(newfd);
	
    exit(EXIT_SUCCESS);
}

