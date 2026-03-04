#include <stdio.h>
#include <stdlib.h>
#include "tracked_malloc.h"

void test_clean_alloc(void){
    printf("==== Test 1: Clean Allocation ====\n");

    void *ptr = malloc(100);
    if (!ptr){
        perror("Memory could not allocated");
        return;
    }

    free(ptr);

    printf("Clean allocation/free done.\n");
}
void test_memory_leak(void){
    printf("==== Test 2: Intentional Leak ====\n");
    
    void *ptr = malloc(100);
    if (!ptr){
        perror("Memory could not allocated");
        return;
    }

    printf("Allocated 100 bytes at %p (not freed)\n", ptr);
}
void test_double_free(void){
    printf("==== Test 3: Double Free ====\n");

    void *ptr = malloc(100);
    if (!ptr){
        perror("Memory could not allocated");
        return;
    }
    
    free(ptr);

    printf("First free done. Attempting second free... \n");

    free(ptr);
}

int main(){
    test_clean_alloc();
    test_memory_leak();
    test_double_free();

    printf("Exiting Main (atexit report can be seen below)\n");
    return 0;
}
