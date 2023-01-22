/**
 * File Name: C1.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 20, 2021
 * Time of Last Modification: 0843, Friday, June 18, 2021
 * Assistance: Lines 30-49 acquired from CMPEN Module 5 sample code.
 * Description: Program reads input from memory stack and outputs
 * to console. Memory stack dataprovided by
 * companion producer program.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
	const char *name = "OS";
	const int SIZE = 4096;

	int shm_fd;
	void *ptr;
	int i;
	
	/* open the shared memory segment */
	shm_fd = shm_open(name, O_RDONLY, 0666);
	if (shm_fd == -1) {
		printf("shared memory failed\n");
		exit(-1);
	}

	/* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		exit(-1);
	}
	
	/* now read from the shared memory region */
	printf("%s",(char *)ptr);

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}
	
	return 0;
}

/**
 * Sample Output: Producer
 * ----
 * Enter single input, i.e., no multiple spaced inputs: hello
 *
 * Do you want to continue? (0) for yes and (1) for no: 0
 * Enter single input, i.e., no multiple spaced inputs: friend
 *
 * Do you want to continue? (0) for yes and (1) for no: 0
 * Enter single input, i.e., no multiple spaced inputs: yessir
 *
 * Do you want to continue? (0) for yes and (1) for no: 1
 * ----
 * Sample Output: Consumer
 * ----
 * hello
 * friend
 * yessir
 **/