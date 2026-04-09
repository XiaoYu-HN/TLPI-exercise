#include <fcntl.h>
#include <unistd.h>
#include "tlpi_hdr.h"


int
main(int argc, char *argv[])
{
	int oldfd, newfd;
	off_t old_offset, new_offset;
	int old_status, new_status;

	if (argc != 2) 
            usageErr("%s filename\n", argv[0]);
    

	oldfd = open(argv[1], O_RDONLY);
	if (oldfd == -1)
		errExit("open");

	newfd = dup(oldfd);		
	if (newfd == -1)
		errExit("dup");
		

	// part 1: compare offset

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
		

	// part 2: compare file status
	old_status = fcntl(oldfd, F_GETFL);
	if (old_status == -1)
		errExit("fcntl F_GETFL");

	new_status = fcntl(newfd, F_GETFL);
	if (new_status == -1)
		errExit("fcntl F_GETFL");

	if (new_status == old_status) 
		printf("file status is the same: %d\n", old_status);
	else
		printf("file status is NOT the same: %d vs %d\n", old_status, new_status);

	
	int ret = fcntl(oldfd, F_SETFL, old_status | O_APPEND);
	if (ret == -1)
		errExit("fcntl F_GETFL");

	old_status = fcntl(oldfd, F_GETFL);
	if (old_status == -1)
		errExit("fcntl F_GETFL");

	new_status = fcntl(newfd, F_GETFL);
	if (new_status == -1)
		errExit("fcntl F_GETFL");

	if (new_status == old_status) 
		printf("file status is the same: %d\n", old_status);
	else
		printf("file status is NOT the same: %d vs %d\n", old_status, new_status);

	close(oldfd);
	close(newfd);
	
    exit(EXIT_SUCCESS);
}

