/**
 * File Name: Queue.c
 * Author: Ahmed Sammoud
 * Modified by: Anthony Vallin, aav5195
 * Time of Last Modification: 1730, Thursday, June 24, 2021
 * Lines Modified: Line 30
 * Description: Provides Queue library functions
 **/

#include<stdio.h>
#include<stdlib.h>
#include"Queue.h"

#define DEBUG
#ifdef DEBUG
#define DEBUG_PRINT printf
#else
#define DEBUG_PRINT(...)
#endif

 void DisplayList(Queue_Info *Q)
 {
    Qnode *t;
         
    if(Q == NULL){
        perror("Queue Header is Null");
    }
    t=Q->first;
     while(t != NULL){
         DEBUG_PRINT(" Data %d \n",t->data, t->cpuTime);
         t = t->next;
     }
 }
 
 Qnode * search(Queue_Info * Q, int data){  
    
    if(Q == NULL){
        perror("Queue Header is Null");
    }
    
    if(Q->Queue == NULL){
        perror("Queue is empty");
    }
     
     Qnode *ptr;
     ptr = Q->Queue;
     
     while( ptr != NULL )
     {
        if(ptr->data == data){
            return ptr;
        }
        ptr = ptr->next;
     }
     
     return NULL;
     
 } 
 
 void Enqueue(Queue_Info *Q, Qnode * p){
     
     if(p == NULL){
         perror("No node to add ");
     }
     
     if(Q->first == NULL){
        Q->Queue = p;
        Q->first = p;
        Q->Last = p;
     }else{

     p->next = NULL;
     Q->Last->next = p; 
     Q->Last = p;
     }
     return;
     
 }
 
 Qnode * Dequeue(Queue_Info *Q){
     
     if(Q == NULL){
         perror("No Queue to search");
     }
     
     if(Q->first == NULL || Q->Last == NULL || Q->Queue == NULL){
         perror("Empty Queue, Nothing to remove");
         return NULL;
     }
     
     Qnode *p = Q->first;
     
     Q->first = Q->first->next;
     Q->Queue = Q->first;
     
     return p;
 }
 
  Qnode* Pop(Queue_Info *S){
     
    if(S== NULL){
        perror("No Stack to Pop");
    }
     
    if(S->first == NULL){
        perror("Empty Queue, Nothing to remove");
        return NULL;
    }
    
    return Dequeue(S);
}