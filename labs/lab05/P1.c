/**
 * File Name: P1.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 20, 2021
 * Time of Last Modification: 0843, Friday, June 18, 2021
 * Assistance: Lines 33-43 and 54-55 acquired from CMPEN Module 5 sample code.
 * Description: Program writes input to the memory stack so that a consumer 
 * program can read and display.
 **/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

int main() 
{
    const int SIZE   = 4096; 
    const char *name = "OS"; //Name of shared memory object
    char userInput[25];      //User input string literal
    
    int exitFlag = 0;
    int shm_fd;              //File descriptor for the shared memory received from an earlier shm_open
    void *ptr;
    
    /* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	
	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,SIZE);
    
    /* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) {
		printf("Map failed\n");
		return -1;
	}
	
	do
	{
		printf("Enter single input, i.e., no multiple spaced inputs: ");
		scanf("%s", &userInput);
		
		/** 
		 *Now write to the shared memory region and
		 * increment the value of ptr after each write.
		 **/
		sprintf(ptr, "%s", userInput);
		ptr += strlen(userInput);
		sprintf(ptr, "%c", '\n');
		ptr++;
		
		printf("\nDo you want to continue? (0) for yes and (1) for no: ");
        scanf("%d", &exitFlag);
	} while(exitFlag != 1);
    
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