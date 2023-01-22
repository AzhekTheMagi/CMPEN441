/**
 * File Name: FactNum.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 06, 2021
 * Time of Last Modification: 1036, Saturday, June 5, 2021
 * Program calculates factorial value of integer.
 **/

#include <stdio.h>

int main() {
    unsigned long long factorial = 1; // Holds factorial value
    int n;                       // Holds user input
    
    //User Input
    printf("Enter real integer number: ");
    scanf("%d", &n);
    
    //Calculates factorial value. Negative numbers not permitted. 
    if (n < 0) {
        printf("Error: Negative value entered\n");
    }
    else {
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }
    }
    
    //Displays results of factorial calculation.
    printf("Factorial of %d = %llu\n", n, factorial);
    
    return 0;
}