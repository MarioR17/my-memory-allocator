#include "heap.h"

int main(void)
{

        char *heap_memory = (char*)malloc(100 * 128);

        if (heap_memory == NULL) {
                return 1;
        }

        return 0;
}
