/**
 * File Name: Queue.h
 * Author: Ahmed Sammoud
 * Modified by: Anthony Vallin, aav5195
 * Time of Last Modification: 1730, Thursday, June 24, 2021
 * Lines Modified: Line 14
 * Description: Provides Queue library header functions
 **/

#ifndef QUEUE_H
#define QUEUE_H
typedef struct Qnode{
    int data;
    int cpuTime;
    struct Qnode * next;
}Qnode;

typedef struct Queue_Info{
    Qnode *first;
    Qnode *Last;
    Qnode *Queue;
}Queue_Info;

void DisplayList(Queue_Info *Q);
 
Qnode * search(Queue_Info * Q,int data);
 
void Enqueue(Queue_Info *Q, Qnode * p);
 
Qnode * Dequeue(Queue_Info *Q);

Qnode* Pop(Queue_Info *S);

#endif