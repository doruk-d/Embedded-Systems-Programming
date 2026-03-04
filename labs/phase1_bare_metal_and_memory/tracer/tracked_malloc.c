#define SYS_MALLOC

#include <stdio.h>
#include <stdlib.h>
#include "tracked_malloc.h"

typedef struct allocation{
    const char *file;
    int line;
    size_t size;
    void *ptr;
    struct allocation *prev;
    struct allocation *next;
}allocation_t;

static allocation_t *head = NULL;
static allocation_t *tail = NULL;

static int registered = 0;

void *tracked_malloc(size_t size, const char *file, int line){
    if (!registered){
        atexit(report_leak);
        registered = 1;
    }

    allocation_t *new_node = malloc(sizeof(allocation_t));
    if (!new_node){
        perror("memory could not allocated");
        return NULL;
    }

    void *ptr = malloc(size);
    if (ptr == NULL){
        perror("memory could not allocated");
        free(new_node);
        return NULL;
    }


    new_node->file = file;
    new_node->line = line;
    new_node->size = size;
    new_node->ptr = ptr;
    new_node->next = NULL;

    if (head == NULL){
        new_node->prev = NULL;
        head = new_node;
        tail = new_node;
    }
    else{
        new_node->prev = tail;
        tail->next = new_node;
        tail = new_node;
    }

    return ptr;
}
void tracked_free(void *ptr, const char *file, int line){
    allocation_t *current = head;

    int found = 0;
    while (current != NULL){
        if (current->ptr == ptr){
            found = 1;
            if (current->prev) current->prev->next = current->next;
            if (current->next) current->next->prev = current->prev;

            if (current == head) head = current->next;
            if (current == tail) tail = current->prev;
            
            fprintf(stderr,"Freed allocation from %s: %d at %s: %d\n", current->file, current->line, file, line);

            free(current);
            break;
        }
        current = current->next;
    }

    if (!found)
        fprintf(stderr,"Double free or invalid pointer at %s: %d\n", file, line);
    
    else
        free(ptr);

    return;
}

void report_leak(void){
    if (head != NULL){
        allocation_t *current = head;
        while (current != NULL){
            fprintf(stderr,"Leak detected: %zu bytes allocated at %s: %d\n", current->size, current->file, current->line);
            allocation_t *next = current->next;
            free(current->ptr);
            free(current);
            current = next;
        }
    }

}
