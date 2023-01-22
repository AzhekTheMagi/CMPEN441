/**
 * File Name: Strlen.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 06, 2021
 * Time of Last Modification: 1036, Saturday, June 5, 2021
 * Program counts number of characters in a string.
 **/

#include <stdio.h>
#include <string.h>

int main() {
    char name[250];
    
    printf("Enter string of characters: ");
    scanf("%s", &name);
    printf("%s contains %i characters\n", name, strlen(name));
    
    return 0;
}