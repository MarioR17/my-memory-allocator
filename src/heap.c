#include <unistd.h>

#include "heap.h"

FreeNode *free_list_head = NULL;

void *malloc(size_t num_bytes)
{
        write(1, "Using this one\n", 15);
        void *heap_start = sbrk(num_bytes);

        if (heap_start == (void*)-1) {
                return NULL;
        }

        return heap_start;
}
