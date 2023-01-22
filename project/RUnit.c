/**
 * File Name: RUnit.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Tuesday, Aug 10, 2021
 * Time of Last Modification: 1730, Friday, July 30, 2021
 * Assistance: Lines 30-49 acquired from CMPEN Module 5 sample code.
 * Description: Displays the tempature (Farenheit) data stored in the
 * shared memory. 
 **/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>

int main()
{
	const char *name = "Calculated_Data";
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
	
	printf("Simulated Farenheit Temperature readings: \n");
	
	/* now read from the shared memory region */
	printf("%s", (char *)ptr);

	/* remove the shared memory segment */
	if (shm_unlink(name) == -1) {
		printf("Error removing %s\n",name);
		exit(-1);
	}
	
	close(shm_fd);
	
    return 0;
}