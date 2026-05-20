#include "heap.h"
#include "print.h"

int main(void)
{
        size_t mem_req = 6;

        print_str("About to malloc for 1st time\n");

        print_str("Memory requested: ");
        print_szt(mem_req);
        print_str("\n");

        char *heap_memory = (char*)malloc(mem_req);

        if (heap_memory == NULL) {
                print_str("Error: Malloc returned a null pointer\n");
                return 1;
        } 

        print_str("first malloc memory address: ");
        print_ptr(heap_memory);
        print_str("\n");

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
        mem_req = 105;

        print_str("Memory requested: ");
        print_szt(mem_req);
        print_str("\n");

        char *heap_mem2 = (char*)malloc(mem_req);

        if (heap_mem2 == NULL) {
                print_str("Error: Malloc returned a null pointer\n");
                return 1;
        }

        print_str("second malloc memory address: ");
        print_ptr(heap_mem2);
        print_str("\n");

        
        print_str("About to malloc for 3rd time\n");
        mem_req = 567;

        print_str("Memory requested: ");
        print_szt(mem_req);
        print_str("\n");

        char *heap_mem3 = (char*)malloc(mem_req);

        if (heap_mem3 == NULL) {
                print_str("Error: Malloc returned a null pointer\n");
                return 1;
        }

        print_str("third malloc memory address: ");
        print_ptr(heap_mem3);
        print_str("\n");

        print_str("About to calloc for 1st time (4th alloc)\n");
        size_t num_items = 1000;
        size_t items_size = sizeof(int);

        print_str("Memory requested: ");
        print_szt(num_items * items_size);
        print_str("\n");

        char *heap_mem4 = (char*)calloc(num_items, items_size);

        if (heap_mem4 == NULL) {
                print_str("Error: Calloc returned a null pointer\n");
                return 1;
        }

        print_str("first calloc (4th alloc)  memory address: ");
        print_ptr(heap_mem4);
        print_str("\n");

        print_str("About to calloc for 2nd time (5th alloc)\n");
        num_items = 10;
        items_size = sizeof(double);

        print_str("Memory requested: ");
        print_szt(num_items * items_size);
        print_str("\n");

        char *heap_mem5 = (char*)calloc(num_items, items_size);

        if (heap_mem5 == NULL) {
                print_str("Error: Calloc returned a null pointer\n");
                return 1;
        }

        print_str("second calloc (5th alloc)  memory address: ");
        print_ptr(heap_mem5);
        print_str("\n");

        return 0;
}
