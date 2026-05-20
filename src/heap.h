#ifndef HEAP_H
#define HEAP_H

#include <stddef.h>
#include <stdbool.h>
#include <unistd.h>

#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))
#define PAGE_SIZE get_cached_page_size()
#define HEADER_SIZE ALIGN(sizeof(struct ChunkHeader))
#define FREE_NODE_SIZE ALIGN(sizeof(FreeNode))

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
void *calloc(size_t num_items, size_t items_size);

static inline int get_cached_page_size(void)
{
        static int cached_page_size = 0;

        if (cached_page_size == 0)
                cached_page_size = sysconf(_SC_PAGESIZE);

        return cached_page_size;
}

#endif /* HEAP_H */
