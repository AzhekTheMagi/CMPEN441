/**
 * File Name: shell.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 06, 2021
 * Time of Last Modification: 1036, Saturday, June 5, 2021
 * Program allows user to open a child process to run an 
 * executable of their choice.
 **/

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

int main() {
    char filePath[] = "/home/ec2-user/environment/aav5195osweb/labs/lab03/";
    char buffer[260]; // Holds concatenated file name with user input
    char name[50];    // Holds user input
    int  pid;         // Process ID of thread group
    
    do {
        printf("Enter program name: ");
        scanf("%s", &name);
        pid = fork();
        
        if (pid == 0) {
            snprintf(buffer, sizeof(buffer), "%s%s", filePath, name);
            execve(buffer, NULL, NULL);
        }
        else if (pid > 0) {
            int status;
        
            wait(&status);
            printf(" >>>Parent<<< \n");
        }
        else {
            printf("Error \n");
        }
    } while(strcmp(name, "end") != 0);

    return 0;
}