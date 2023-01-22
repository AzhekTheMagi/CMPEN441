/**
 * File Name: Sort_Numbers.c
 * Name: Anthony Vallin, aav5195
 * Course Title: CMPEN 441
 * Due: 2359, Sunday, June 13, 2021
 * Time of Last Modification: 0910, Wednesday, June 9, 2021
 * Program takes a list of unsorted integers from a user, sorts them from 
 * least to max, and displays sorted integers. Operation for number sort and
 * display is done using threads.
 **/
 
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int const ARRAYSIZE = 5;
int values[5];

/**
 * Compare and quicksort algorithm acquired from cplusplus.com
 * URL: https://www.cplusplus.com/reference/cstdlib/qsort/
 **/
int compare(const void * a, const void * b) {
    return (*(int*)a - *(int*)b);
}

void * SortNumbers(void * arg) {
    qsort(values, ARRAYSIZE, sizeof(int), compare);
    for(int i = 0; i < ARRAYSIZE; i++) {
        printf("%d ", values[i]);
    }
    
    return 0;
}
 
int main() {
    int exitFlag = 0;
    int result_t;
    
    do {
        printf("Enter 5 numbers: ");
        for (int i = 0; i < ARRAYSIZE; i++) {
            scanf("%d", &values[i]);
        }
        
        pthread_t Thread1;
        result_t = pthread_create(&Thread1, NULL, SortNumbers, NULL);
        
        if (result_t != 0) {
            perror("Thread1 creation failed");
        }
        
        void * return_val;
        result_t = pthread_join(Thread1, return_val);
        
        if (result_t != 0) {
            perror("Thread1 join failed");
        }
        
        printf("\nDo you want to continue? (0) for yes and (1) for no: ");
        scanf("%d", &exitFlag);
    } while(exitFlag != 1);
    
    return 0;
}

/**
 * Sample Output: 
 * 
 * vocstartsoft:~/environment/aav5195osweb/labs/lab04 $ ./output
 * Enter 5 numbers: 1 50 35 20 8
 * 1 8 20 35 50 
 * Do you want to continue? (0) for yes and (1) for no: 0
 * Enter 5 numbers: 500 29 65 8 100
 * 8 29 65 100 500 
 * Do you want to continue? (0) for yes and (1) for no: 1
 * vocstartsoft:~/environment/aav5195osweb/labs/lab04 $
 **/