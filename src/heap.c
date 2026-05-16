#include <stdio.h>
#include <unistd.h>

#include "heap.h"

void *malloc(size_t num_bytes) {
        printf("HIIII\n");
        void *heap_start = sbrk(0);

        heap_start = (void*)-1;
        printf("HIIII\n");
        if (heap_start == (void*)-1) {
                printf("HIIII\n");
                perror("sbrk");
                return NULL;
        }

        return heap_start;
}
