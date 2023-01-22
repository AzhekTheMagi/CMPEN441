//---------------------------------------------------------------
// File: Stack.h
// Purpose: Header file for a demonstration of a stack implemented 
//		as a linked structure.  Data type: Character
// Programming Language: C
// Author: Dr. Rick Coleman
// Modified by: Anthony Vallin, aav
// Date Modified: 20210701
// Modification: Modified struct datatype to int
//---------------------------------------------------------------
#ifndef STACK_H
#define STACK_H

#include <stdio.h>

// Define a structure to be used as the stack item
struct StackItem
{
	int    data;
	struct StackItem *next;
} StackItem;

// List Function Prototypes
void InitStack();         // Initialize the stack
void ClearStack();        // Remove all items from the stack
int Push(int data);       // Push an item onto the stack
int Pop();               // Pop an item from the stack
int isEmpty();            // Return true if stack is empty
int isFull();             // Return true if stack is full

// Define TRUE and FALSE if they have not already been defined
#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE (!FALSE)
#endif

#endif // End of stack header