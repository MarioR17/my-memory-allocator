#include <stdio.h>

#include "heap.h"

int main(void)
{

        char *heap_memory = (char*)malloc(100 * 128);

        if ((char*)NULL == (void*)NULL) {
                printf("char* NULL == void* NULL\n");
        }

        if (heap_memory == NULL) {
                perror("malloc returned null");
                return 1;
        }

        printf("Address of heap_memory: %p\n", heap_memory);

        heap_memory[1] = 'a';
        heap_memory[2] = '\0';
        printf("%c\n", heap_memory[1]);

        return 0;
}
