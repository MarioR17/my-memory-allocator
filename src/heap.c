#include <unistd.h>

#include "heap.h"
#include "print.h"

FreeNode *free_list_head = NULL;

void *malloc(size_t num_bytes)
{
        if (num_bytes == 0) return NULL;

        size_t aligned_payload_size = ALIGN(num_bytes);

        print_str("Using our malloc...\n");
        print_str("Size requested: ");
        print_szt(aligned_payload_size);
        print_str("\n");


        void *heap_start = sbrk(aligned_payload_size);

        if (heap_start == (void*)-1) {
                print_str("sbrk failed\n");
                return NULL;
        }

        print_str("Address of heap_start: ");
        print_ptr(heap_start);
        print_str("\n");

        return heap_start;
}
