/**
 * File Name: Deadlock.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, July 11, 2021
 * Time of Last Modification: 0925, Wednesday, July 7, 2021
 * Description: Program demonstrates a simple deadlock.  
 **/

#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include<pthread.h>

unsigned int sleep(unsigned int seconds);

int MAX = 100;
int MIN = 0;

int V1;
int V2;

time_t t;

pthread_mutex_t first_mutex;
pthread_mutex_t second_mutex;

void * Thread_One(void * arg);
void * Thread_Two(void * arg);

int main()
{
    srand((unsigned) time(&t));
    
    pthread_mutex_init(&first_mutex, NULL);
    pthread_mutex_init(&second_mutex, NULL);
    
    pthread_t thread1, thread2;
    void *ret;
    int result;
    
    result = pthread_create(&thread1, NULL, Thread_One, NULL);
    
    if (result != 0)
		perror("Thread creation failed");
		
	result = pthread_create(&thread2, NULL, Thread_Two, NULL);
	
	if (result != 0)
		perror("Thread creation failed");
    
    pthread_join(thread1, &ret);
    pthread_join(thread2, &ret);
    
    return 0;
}

void * Thread_One(void * arg)
{
    pthread_mutex_lock(&first_mutex);
    sleep(2);
    
    pthread_mutex_lock(&second_mutex);
    
    V1 = (rand() % (MAX - MIN + 1) + MIN);
    printf("Value in V2: %i \n", V2);
    
    pthread_mutex_unlock(&second_mutex);
    pthread_mutex_unlock(&first_mutex);
}

void * Thread_Two(void * arg)
{
    pthread_mutex_lock(&second_mutex);
    sleep(2);
    
    pthread_mutex_lock(&first_mutex);
    
    printf("Value in V1: %i \n", V1);
    V2 = (rand() % (MAX - MIN + 1) + MIN);
    
    pthread_mutex_unlock(&first_mutex);
    pthread_mutex_unlock(&second_mutex);
}

/**
 * Sample Output
 * ----
vocstartsoft:~/environment/aav5195osweb/labs/lab08 $ ./lab08Output

* ----
**/