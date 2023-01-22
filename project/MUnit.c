/**
 * File Name: MUnit.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Tuesday, Aug 10, 2021
 * Time of Last Modification: 1727, Friday, July 30, 2021
 * Assistance: Module 15 project hints flowchart.
 * Description: Reads integers from the user to simulate 
 * temperature readings from a sensor. Program uses 4 threads
 * to obtain, store, process, and send data to the shared memory.
 **/

#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/types.h>
#include "Queue.h"

#define SIZE 5
#define MEMORY_SIZE 4096

pthread_mutex_t Lock_Input; //Locks access to the 'Input' variable
pthread_mutex_t Lock_PList; //Locks access to the 'Processing_Queue'.
pthread_mutex_t Lock_SList; //Locks access to the 'Sending_Queue'.

sem_t Input_Ready;   //Binary semaphore indicates when main thread has reading.
sem_t Input_Taken;   //Binary semaphore indicates thread has taken the input.
sem_t Empty_PSlots;  //Counting semaphore indicates Queue slot availability.
sem_t Filled_PSlots; //Counting semaphore indicates when item added to Queue.
sem_t Empty_SSlots;  //Counting sem indicates slot availability in Node thread.
sem_t Filled_SSlots; //Counting sema indicates item added to 'Sending_Queue'.

Queue_Info Processing_Queue; //Queue populated with celsius temps
Queue_Info Sending_Queue;    //Queue populated with converted farehnheit temps

const char *name = "Calculated_Data"; //Shared memory name
int end_time = 0; //Thread loop exit flag
int Input = 0;    //Holds simulated Celsius temps

void *Add_Processing_Queue(void *arg);
void *Processing_Nodes(void *arg);
void *Send_To_Smemory(void *arg);

void *ptr;
void *ret;

int main()
{
    int shm_fd;
    int result_t1, result_t2, result_t3;
    
    pthread_t Add_Processing_Queue_Thread, Process_Nodes_Thread;
    pthread_t Send_To_Smemory_Thread;
    
    Processing_Queue.first = NULL;
    Processing_Queue.Last  = NULL;
    Processing_Queue.Queue = NULL;
    Processing_Queue.count = 0;
    
    Sending_Queue.first = NULL;
    Sending_Queue.Last  = NULL;
    Sending_Queue.Queue = NULL;
    Sending_Queue.count = 0;
    
    pthread_mutex_init(&Lock_Input, NULL);
    pthread_mutex_init(&Lock_PList, NULL);
    pthread_mutex_init(&Lock_SList, NULL);
    
    sem_init(&Input_Ready, 0, 0);
    sem_init(&Input_Taken, 0, 1);
    
    sem_init(&Empty_PSlots,  0, 5);
    sem_init(&Filled_PSlots, 0, 0);
    
    sem_init(&Empty_SSlots,  0, 5);
    sem_init(&Filled_SSlots, 0, 0);
    
    /* create the shared memory segment */
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	
	/* configure the size of the shared memory segment */
	ftruncate(shm_fd,MEMORY_SIZE);
    
    /* now map the shared memory segment in the address space of the process */
	ptr = mmap(0,MEMORY_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if (ptr == MAP_FAILED) 
		printf("Map failed\n");
    
    while (Input != -1)
    {
        sem_wait(&Input_Taken);
        pthread_mutex_lock(&Lock_Input);
        
        printf("Enter simulated CPU temp in Celsius: ");
        scanf("%i", &Input);
		
		sem_post(&Input_Ready);
		pthread_mutex_unlock(&Lock_Input);
		
		result_t1 = pthread_create(&Add_Processing_Queue_Thread, NULL,
		                           Add_Processing_Queue, NULL);
		
		result_t2 = pthread_create(&Process_Nodes_Thread, NULL,
		                           Processing_Nodes, NULL);
	
		result_t3 = pthread_create(&Send_To_Smemory_Thread, NULL,
		                           Send_To_Smemory, NULL);
    }

    end_time = -1;
    
    close(shm_fd);
    
    pthread_join(Add_Processing_Queue_Thread, &ret);
    pthread_join(Process_Nodes_Thread, &ret);
    pthread_join(Send_To_Smemory_Thread, &ret);
    
    return 0;
}

//--------------------------------------------------------
// Function: Add_Processing_Queue()
// Purpose: Gets reading from "Input" and stores it into
//     'Processing_Queue' queue.
// Precondition: Queue can only accept five values at a
//     time, and thread will wait if more data needs to be
//     written.
// Returns: void
//--------------------------------------------------------
void *Add_Processing_Queue(void *arg)
{
    Qnode *tmp;
    
    while (end_time != -1)
    {
        sem_wait(&Input_Ready);
        sem_wait(&Empty_PSlots);
        pthread_mutex_lock(&Lock_Input);
    
        tmp = (Qnode *) malloc(sizeof(Qnode));
	    tmp->data = Input;
	    
	    pthread_mutex_unlock(&Lock_Input);
	    pthread_mutex_lock(&Lock_PList);
	    
	    Enqueue(&Processing_Queue, tmp);
	    Processing_Queue.count++;
	    
	    pthread_mutex_unlock(&Lock_PList);
	    sem_post(&Input_Taken);
	    sem_post(&Filled_PSlots);
    }
    
    end_time = -1;
}

//------------------------------------------------------------
// Function: Processing_Nodes()
// Purpose: Gets data from the 'Processing_Queue' and
//     converts temperature from celsius to farenheit.
//     The result is stored in another queue, 'Sending_Queue'.
//     Celsius to Farenheit conversion formula: (C * 1.8) + 32
// Returns: void
//------------------------------------------------------------
void *Processing_Nodes(void *arg)
{
    int fahrenheit_temp;
    
    Qnode *processing_tmp;
    Qnode *sending_tmp;
    
    while (end_time != -1)
    {
        sem_wait(&Filled_PSlots);
        pthread_mutex_lock(&Lock_PList);
        
        processing_tmp = (Qnode *) malloc(sizeof(Qnode));
        processing_tmp = Dequeue(&Processing_Queue);

        pthread_mutex_unlock(&Lock_PList);
        sem_post(&Empty_PSlots);
        
        fahrenheit_temp = (processing_tmp->data * 1.8) + 32;
        
        sem_wait(&Empty_SSlots);
        pthread_mutex_lock(&Lock_SList);

        sending_tmp = (Qnode *) malloc(sizeof(Qnode));
        sending_tmp->data = fahrenheit_temp;
        Enqueue(&Sending_Queue, sending_tmp);

        pthread_mutex_unlock(&Lock_SList);
        sem_post(&Filled_SSlots);
    }
}

//---------------------------------------------------
// Function: Send_To_Smemory()
// Purpose: Gets data from "Sending_Queue" and writes
//     the data into a shared memory location.
// Returns: void
//---------------------------------------------------
void *Send_To_Smemory(void *arg)
{
    Qnode *tmp;
    char value[SIZE];
    
    while (end_time != -1)
    {
        sem_wait(&Filled_SSlots);
        pthread_mutex_lock(&Lock_SList);
        
        tmp = (Qnode *) malloc(sizeof(Qnode));
        tmp = Dequeue(&Sending_Queue);

        sprintf(value, "%i", tmp->data);
        
        sprintf(ptr, "%s", value);
        ptr += strlen(value);
        sprintf(ptr, "%c", '\n');
        ptr++;

        pthread_mutex_unlock(&Lock_SList);
        sem_post(&Empty_SSlots);
    }
}

/**
 * Sample Output for MUnit and RUnit
 * -----
vocstartsoft:~/environment/aav5195osweb/project $ ./MUnitTest
Enter simulated CPU temp in Celsius: 25
Enter simulated CPU temp in Celsius: 14
Enter simulated CPU temp in Celsius: 10
Enter simulated CPU temp in Celsius: 9
Enter simulated CPU temp in Celsius: 0
Enter simulated CPU temp in Celsius: 50
Enter simulated CPU temp in Celsius: -1
vocstartsoft:~/environment/aav5195osweb/project $ ./RUnitTest
Simulated Farenheit Temperature readings:
77
57
50
48
32
122
30
vocstartsoft:~/environment/aav5195osweb/project $
**/