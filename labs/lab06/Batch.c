/**
 * File Name: Batch.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 27, 2021
 * Time of Last Modification: 1730, Thursday, June 24, 2021
 * Assistance: MergeSort Algorithm (lines 117-204) acquired from 
 * url: https://www.techiedelight.com/merge-sort-singly-linked-list/
 * Description: Program demonstrates First Come First Serve
 * and Shortest Job Algorithms.
 **/

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"Queue.h"

struct Qnode* sortedMerge(struct Qnode* a, struct Qnode* b);
void frontBackSplit(struct Qnode* source, struct Qnode** frontRef,
                    struct Qnode** backRef);
void mergeSort(struct Qnode** head);
void* FCFS(void* arg);
void* SJF(void* arg);

int main() 
{
    int x, y;
    int result_t1, result_t2;
    
    Qnode * tmp;
    Queue_Info QH;
    QH.first = NULL;
    QH.Last = NULL;
    QH.Queue = NULL;
   
    do
    {
        printf("Enter an ID and CPU finish time (Enter -1 to stop): ");
        scanf("%d %d", &x, &y);
        
        if (x == -1 && y == -1)
            break;

        tmp = (Qnode *) malloc(sizeof(Qnode));
        tmp->data = x;
        tmp->cpuTime = y;
        Enqueue(&QH, tmp);
    } while(x != -1);
    
    pthread_t fcfsThread, sjfThread;
    
    result_t1 = pthread_create(&fcfsThread, NULL, FCFS, &QH);
    
    if(result_t1 != 0)
        perror("Thread Creation failed");
        
    pthread_join(fcfsThread, NULL);
    
    result_t2 = pthread_create(&sjfThread, NULL, SJF, &QH);
    
    if(result_t2 != 0)
        perror("Thread Creation failed");
        
    pthread_join(sjfThread, NULL);
    
    printf("All Threads completed \n");
    
    return 0;
}

void* FCFS(void* arg) 
{
    Queue_Info *Q = (Queue_Info*)arg;
    Qnode *t;
    
    printf("\nFCFS Job order: \n");
    printf("FCFS Jobs will be executed in the order they arrived: \n");
    printf("Linked List contain the following values: \n");
    
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
    
    printf("FCFS done \n");
    free(t);
}

void* SJF(void* arg)
{
    Queue_Info *Q = (Queue_Info*)arg;
    Qnode *t;
    
    if(Q == NULL)
    {
        perror("Queue Header is Null");
    }
    
    t = Q->first;
    mergeSort(&t);
    
    printf("\nSJF Jobs Order: \n");
    
    while(t != NULL)
    {
        printf("Shortest Job to Run next is with ID: %d and Time %d \n", t->data, t->cpuTime);
        t = t->next;
    }
    
    printf("SJF: All Jobs Completed \n");
    free(t);
}

// Takes two lists sorted in increasing order and merge their nodes
// to make one big sorted list, which is returned
struct Qnode* sortedMerge(struct Qnode* a, struct Qnode* b)
{
    // base cases
    if (a == NULL) {
        return b;
    }
 
    else if (b == NULL) {
        return a;
    }
 
    struct Qnode* result = NULL;
 
    // pick either `a` or `b`, and recur
    if (a->cpuTime <= b->cpuTime)
    {
        result = a;
        result->next = sortedMerge(a->next, b);
    }
    else {
        result = b;
        result->next = sortedMerge(a, b->next);
    }
 
    return result;
}

/*
    Split the given list's nodes into front and back halves
    and return the two lists using the reference parameters.
    If the length is odd, the extra node should go in the front list.
    It uses the fast/slow pointer strategy
*/
void frontBackSplit(struct Qnode* source, struct Qnode** frontRef,
                    struct Qnode** backRef)
{
    // if the length is less than 2, handle it separately
    if (source == NULL || source->next == NULL)
    {
        *frontRef = source;
        *backRef = NULL;
        return;
    }
 
    struct Qnode* slow = source;
    struct Qnode* fast = source->next;
 
    // advance `fast` two nodes, and advance `slow` one node
    while (fast != NULL)
    {
        fast = fast->next;
        if (fast != NULL)
        {
            slow = slow->next;
            fast = fast->next;
        }
    }
 
    // `slow` is before the midpoint in the list, so split it in two
    // at that point.
    *frontRef = source;
    *backRef = slow->next;
    slow->next = NULL;
}
 
// Sort a given linked list using the merge sort algorithm
void mergeSort(struct Qnode** head)
{
    // base case — length 0 or 1
    if (*head == NULL || (*head)->next == NULL) {
        return;
    }
 
    struct Qnode* a;
    struct Qnode* b;
 
    // split `head` into `a` and `b` sublists
    frontBackSplit(*head, &a, &b);
 
    // recursively sort the sublists
    mergeSort(&a);
    mergeSort(&b);
 
    // answer = merge the two sorted lists
    *head = sortedMerge(a, b);
}

/**
 * Sample Output:
 * ----
 * Enter an ID and CPU finish time (Enter -1 to stop): 1 200
 * Enter an ID and CPU finish time (Enter -1 to stop): 2 30
 * Enter an ID and CPU finish time (Enter -1 to stop): 4 300
 * Enter an ID and CPU finish time (Enter -1 to stop): -1 -1
 *
 * FCFS Job order: 
 * FCFS Jobs will be executed in the order they arrived: 
 * Linked List contain the following values: 
 * Node ID 1 with value 200 
 * Node ID 2 with value 30 
 * Node ID 4 with value 300 
 * FCFS done 
 *
 * SJF Jobs Order: 
 * Shortest Job to Run next is with ID: 2 and Time 30 
 * Shortest Job to Run next is with ID: 1 and Time 200 
 * Shortest Job to Run next is with ID: 4 and Time 300 
 * SJF: All Jobs Completed
 **/