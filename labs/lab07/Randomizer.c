#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include "Stack.h"

int MAX_SIZE = 5;
int idStack[5];
int ID = 0;

sem_t Empty_Slots;
sem_t Filled_Slots;

pthread_mutex_t LOCK;

void * Randomizer(void *arg);
void * Consumer(void *arg);

int main()
{
    InitStack();
    
    pthread_mutex_init(&LOCK, NULL);
    
    pthread_t randomizerThread, consumerThread;
    void *ret;
    int result;
    
    sem_init(&Empty_Slots, 0, 15);  //Assuming stack holds five slots
	sem_init(&Filled_Slots, 0, 0); //No characters in beginning of program
	
	result = pthread_create(&randomizerThread, NULL, Randomizer, NULL);
	
	if (result != 0)
		perror("Thread creation failed");
	
	result = pthread_create(&consumerThread, NULL, Consumer, NULL);
	
	if (result != 0)
		perror("Thread creation failed");
		
    pthread_join(randomizerThread, &ret);
	printf("Completed Producing ... Exit \n");
	pthread_join(consumerThread, &ret);
	printf("Completed Consuming ... Exit \n");
	
    return 0;
}

void * Randomizer(void *arg)
{
    int MAX_LOOP_ITERATION = 15;
    int counter = 0;
    int max = 100;
    int min = 0;
    int tmp = 0;
    
    while (1)
    {
        sem_wait(&Empty_Slots);
        pthread_mutex_lock(&LOCK);
        
        tmp = (rand() % (max - min + 1) + min);
        printf("Randomizer Thread >> RAND Generated %i << done. \n", tmp);
        Push(tmp);
        
        sem_post(&Filled_Slots);
        pthread_mutex_unlock(&LOCK);
        
        counter++;
        
        if (counter == MAX_LOOP_ITERATION)
            break;
    }
    
    pthread_exit("Done Consuming \n");
}

void * Consumer(void *arg)
{
    int MAX_LOOP_ITERATION = 15;
    int counter = 0;
    
    while (1)
    {
        sem_wait(&Filled_Slots);
        pthread_mutex_lock(&LOCK);
        
        printf("Consume Thread >> RAND Found %i << Done. \n", Pop());
        
        sem_post(&Empty_Slots);
        pthread_mutex_unlock(&LOCK);
        
        counter++;
        
        if (counter == MAX_LOOP_ITERATION)
            break;
    }
    
    pthread_exit("Done Consuming \n");
}