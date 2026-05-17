#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define PAGE_SIZE get_cached_page_size()

struct ChunkHeader {
        size_t size;
        bool is_free;
};

typedef struct FreeListNode {
        struct FreeListNode *next;
        struct FreeListNode *prev;
} FreeNode;

extern FreeNode *free_list_head;

void *malloc(size_t num_bytes);
void *os_mem_request(size_t num_bytes);

static inline int get_cached_page_size(void)
{
        static int cached_page_size = 0;

        if (cached_page_size == 0)
                cached_page_size = sysconf(_SC_PAGESIZE);

        return cached_page_size;
}
