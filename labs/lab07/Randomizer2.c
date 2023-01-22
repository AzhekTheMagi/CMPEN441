/**
 * File Name: Randomizer.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, July 4, 2021
 * Time of Last Modification: 1700, Thursday, July 1st, 2021
 * Description: Program simulates a program that produces random
 * ID's and prints them using threads for each respective action. 
 **/

#include<time.h>
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>

int stack[5];
int top = -1;
int MAX_SIZE = 5;

time_t t;

sem_t Empty_Slots;
sem_t Filled_Slots;

pthread_mutex_t LOCK;

void * Randomizer(void *arg);
void * Consumer(void *arg);

int pop();
int peak();
int isFull();
int isEmpty();
int push(int data);

int main()
{
    srand((unsigned) time(&t));
    
    pthread_mutex_init(&LOCK, NULL);
    
    pthread_t randomizerThread, consumerThread;
    void *ret;
    int result;
    
    sem_init(&Empty_Slots, 0, 15);  //Assuming stack holds five slots
	sem_init(&Filled_Slots, 0, 0);  //No characters in beginning of program
	
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
        push(tmp);
        
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
        
        printf("Consume Thread >> RAND Found %i << Done. \n", pop());
        
        sem_post(&Empty_Slots);
        pthread_mutex_unlock(&LOCK);
        
        counter++;
        
        if (counter == MAX_LOOP_ITERATION)
            break;
    }
    
    pthread_exit("Done Consuming \n");
}

int pop()
{
    int data;
    
    if (!isEmpty()) 
    {
        data = stack[top];
        top--;
        return data;
    } 
    else
    {
        printf("Stack is empty.\n");
    }
}

int peak()
{
    return stack[top];
}

int isFull()
{
    if (top == MAX_SIZE)
        return 1;
    else
        return 0;
}

int isEmpty()
{
    if (top == -1)
      return 1;
   else
      return 0;
}

int push(int data)
{
    if (!isFull()) 
    {
        top++;   
        stack[top] = data;
    } 
    else
    {
        printf("Stack is full.\n");
    }
}

/**
 * Sample Output
 * -----
vocstartsoft:~/environment/aav5195osweb/labs/lab07 $ ./lab07Output2
Randomizer Thread >> RAND Generated 72 << done. 
Consume Thread >> RAND Found 72 << Done. 
Randomizer Thread >> RAND Generated 98 << done. 
Consume Thread >> RAND Found 98 << Done. 
Randomizer Thread >> RAND Generated 72 << done. 
Randomizer Thread >> RAND Generated 51 << done. 
Randomizer Thread >> RAND Generated 29 << done. 
Randomizer Thread >> RAND Generated 5 << done. 
Consume Thread >> RAND Found 5 << Done. 
Consume Thread >> RAND Found 29 << Done. 
Consume Thread >> RAND Found 51 << Done. 
Consume Thread >> RAND Found 72 << Done. 
Randomizer Thread >> RAND Generated 30 << done. 
Randomizer Thread >> RAND Generated 84 << done. 
Randomizer Thread >> RAND Generated 88 << done. 
Randomizer Thread >> RAND Generated 43 << done. 
Randomizer Thread >> RAND Generated 49 << done. 
Randomizer Thread >> RAND Generated 98 << done. 
Consume Thread >> RAND Found 98 << Done. 
Consume Thread >> RAND Found 49 << Done. 
Consume Thread >> RAND Found 43 << Done. 
Consume Thread >> RAND Found 88 << Done. 
Consume Thread >> RAND Found 84 << Done. 
Consume Thread >> RAND Found 30 << Done. 
Randomizer Thread >> RAND Generated 92 << done. 
Randomizer Thread >> RAND Generated 32 << done. 
Randomizer Thread >> RAND Generated 24 << done. 
Consume Thread >> RAND Found 24 << Done. 
Consume Thread >> RAND Found 32 << Done. 
Consume Thread >> RAND Found 92 << Done. 
Completed Producing ... Exit 
Completed Consuming ... Exit 
**/