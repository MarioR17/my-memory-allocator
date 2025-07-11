#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stddef.h>

void *my_alloc(size_t size)
{
    printf("Allocating %zu bytes\n", size);

    void *ptr = mmap(NULL,
                        size,
                        PROT_READ | PROT_WRITE,
                        MAP_PRIVATE | MAP_ANONYMOUS,
                        -1,
                        0);

    if (ptr == MAP_FAILED)
    {
        perror("mmap failed");
        return NULL;
    }

    printf("mmap returned %p\n", ptr);
    printf("Address range: %p - %p\n", ptr, (char*)ptr + size - 1);
    printf("Size in hex: 0x%zx\n", size);
    
    return ptr;
}

int my_free(void *ptr, size_t size)
{
    int result = munmap(ptr, size);
    if (result == 0)
    {
        printf("Memory at %p of size %zu bytes freed successfully\n", ptr, size);
        return 0;
    } else {
        perror("munmap failed");
        return -1;
    }

    return result;
}

 int main() {
    printf("Page size: %ld bytes (0x%lx)\n", getpagesize(), getpagesize());
    printf("Process ID: %d\n", getpid());
    
    // Show current program break (if available)
    void *current_brk = sbrk(0);
    printf("Current program break: %p\n", current_brk);
    
    printf("\n==================================================\n");  // ← Fixed
    printf("Testing different allocation sizes:\n");
    
    // Small allocation
    void *ptr1 = my_alloc(1024);  // 1KB
    
    // Medium allocation  
    void *ptr2 = my_alloc(8192);  // 8KB (2 pages)
    
    // Large allocation
    void *ptr3 = my_alloc(1048576); // 1MB
    
    // Show the addresses are not contiguous (unlike sbrk)
    printf("\n==================================================\n");  // ← Fixed
    printf("Address comparison:\n");
    printf("ptr1: %p\n", ptr1);
    printf("ptr2: %p\n", ptr2);  
    printf("ptr3: %p\n", ptr3);
    
    if (ptr1 && ptr2) {
        ptrdiff_t diff = (char*)ptr2 - (char*)ptr1;
        printf("ptr2 - ptr1 = %td bytes (0x%tx)\n", diff, diff);
    }
    
    if (ptr2 && ptr3) {
        ptrdiff_t diff = (char*)ptr3 - (char*)ptr2;
        printf("ptr3 - ptr2 = %td bytes (0x%tx)\n", diff, diff);
    }
    
    // Test writing to the memory
    printf("\n==================================================\n");  // ← Fixed
    printf("Testing memory access:\n");
    
    if (ptr1) {
        *(int*)ptr1 = 0xDEADBEEF;
        printf("Wrote 0xDEADBEEF to ptr1, read back: 0x%X\n", *(int*)ptr1);
    }   
    return 0;
}
