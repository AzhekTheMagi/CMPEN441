/**
 * File Name: Fork.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 13, 2021
 * Time of Last Modification: 1036, Saturday, June 5, 2021
 * Program demonstrates a fork action between parent and child.
 **/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int pid = fork();
    
    if (pid == 0) {
        printf(" This is a child process \n");
    }
    else if (pid > 0) {
        int status;
    
        wait(&status);
        printf(" This is a parent process \n");
    }
    else {
        printf("Error \n");
    }
    
    return 0;
}