#include "heap.h"
#include "print.h"

int main(void)
{
        size_t mem_req = 6;

        print_str("Memory requested: ");
        print_szt(mem_req);
        print_str("\n");

        print_str("About to malloc for 1st time\n");
        char *heap_memory = (char*)malloc(mem_req);

        if (heap_memory == NULL) {
                print_str("Error: Malloc returned a null pointer\n");
                return 1;
        } 

        heap_memory[0] = 'h';
        heap_memory[1] = 'e';
        heap_memory[2] = 'l';
        heap_memory[3] = 'l';
        heap_memory[4] = 'o';
        heap_memory[5] = '\0';

        print_str("Value we used with the malloc'd memory: ");
        print_str(heap_memory);
        print_str("\n");

        print_str("About to malloc for 2nd time\n");
        char *heap_mem2 = (char*)malloc(105);

        if (heap_mem2 == NULL) {
                print_str("Error: Malloc returned a null pointer\n");
                return 1;
        }

        return 0;
}
