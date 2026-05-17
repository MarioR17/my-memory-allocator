#include "heap.h"
#include "print.h"

int main(void)
{
        size_t mem_req = 5;

        print_str("Memory requested: ");
        print_szt(mem_req);
        print_str("\n");

        char *heap_memory = (char*)malloc(mem_req);

        if (heap_memory == NULL) {
                return 1;
        }

        return 0;
}
