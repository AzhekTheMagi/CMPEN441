/**
 * File Name: Interactive.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 27, 2021
 * Time of Last Modification: 1040, Friday, June 25, 2021
 * Assistance: Swap function (lines 117-140) acquired from 
 * url: https://www.techiedelight.com/move-last-node-to-front-linked-list/
 * Description: Program demonstrates Round Robin algorithm.
 **/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"Queue.h"

int quantum;

void swap(struct Qnode** head, struct Qnode** position);
void* RoundRobin(void* arg);

int main() 
{
    int x, y;
    int result_t;
    
    Qnode * tmp;
    Queue_Info QH;
    QH.first = NULL;
    QH.Last = NULL;
    QH.Queue = NULL;
    
    printf("Round Robin simulation program \n");
    printf("Enter a quantum Value to be used: ");
    scanf("%d", &quantum);
       
    do
    {
        printf("\nEnter an ID and a value for a node to add to the list, Enter -1 to stop: ");
        scanf("%d %d", &x, &y);
        
        if (x == -1 && y == -1)
            break;

        tmp = (Qnode *) malloc(sizeof(Qnode));
        tmp->data = x;
        tmp->cpuTime = y;
        Enqueue(&QH, tmp);
    } while(x != -1);
    
    pthread_t roundRobinThread;
    result_t = pthread_create(&roundRobinThread, NULL, RoundRobin, &QH);
    
    if(result_t != 0)
        perror("Thread Creation failed");
        
    pthread_join(roundRobinThread, NULL);
    
    return 0;
}

void* RoundRobin(void* arg)
{
    Queue_Info *Q = (Queue_Info*)arg;
    Qnode *t;
    int exitFlag = 0;
    
    printf("\nRound Robin will start on the following list: \n\n");
    printf("Linked list contain the following values: \n\n");

    if(Q == NULL)
    {
        perror("Queue Header is Null");
    }
    
    t = Q->first;
    
    while(t != NULL)
    {
        printf("Node ID %d with value %d \n", t->data, t->cpuTime);
        t = t->next;
    }
    
    printf("\n\n\n");
    t = Q->first;
    
    do
    {
        while (t != NULL)
        {
            t->cpuTime -= quantum;
            printf("Current node %d : time value before %d, after quantum %d \n\n",
                   t->data, t->cpuTime + quantum, t->cpuTime);
    
            if (t->cpuTime <= 0)
            {
                printf("###################\n\n");
                printf("Job with ID %d and time %d removed.\n\n", t->data, t->cpuTime);
                printf("###################\n\n");
                
                swap(&(Q->first),&t);
                Dequeue(Q);
                
                printf("Iteration completed, RoundRobin will start from beginning of the list\n\n");
            }
            t = t->next;
        }
        
        t = Q->first;
        if(Q->Queue == NULL)
            exitFlag = -1;
        
    } while(exitFlag != -1);
    
    printf("RoundRobin:: All jobs completed \n\n");
}

// Function to move the last node to the front of a given linked list
void swap(struct Qnode** head, struct Qnode** position)
{
    // proceed only when the list is valid
    if (!*head || !(*head)->next) {
        return;
    }
 
    struct Qnode* ptr = *head;
 
    // move to the second last node
    while (ptr->next->next) {
        ptr = ptr->next;
    }
 
    // transform the list into a circular list
    ptr->next->next = *head;
 
    // Fix the head pointer
    *head = ptr->next;
 
    // break the chain
    ptr->next= NULL;
}

/**
 * Sample Output:
 * -----
Round Robin simulation program 
Enter a quantum Value to be used: 2

Enter an ID and a value for a node to add to the list, Enter -1 to stop: 1 5

Enter an ID and a value for a node to add to the list, Enter -1 to stop: 2 4

Enter an ID and a value for a node to add to the list, Enter -1 to stop: 3 1

Enter an ID and a value for a node to add to the list, Enter -1 to stop: -1 -1

Round Robin will start on the following list: 

Linked list contain the following values: 

Node ID 1 with value 5 
Node ID 2 with value 4 
Node ID 3 with value 1 



Current node 1 : time value before 5, after quantum 3 

Current node 2 : time value before 4, after quantum 2 

Current node 3 : time value before 1, after quantum -1 

###################

Job with ID 3 and time -1 removed.

###################

Iteration completed, RoundRobin will start from beginning of the list

Current node 1 : time value before 3, after quantum 1 

Current node 2 : time value before 2, after quantum 0 

###################

Job with ID 2 and time 0 removed.

###################

Iteration completed, RoundRobin will start from beginning of the list

Current node 1 : time value before 1, after quantum -1 

###################

Job with ID 1 and time -1 removed.

###################

Iteration completed, RoundRobin will start from beginning of the list

RoundRobin:: All jobs completed
----
**/