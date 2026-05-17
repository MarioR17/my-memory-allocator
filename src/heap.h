#include <stddef.h>
#include <stdbool.h>

#define ALIGNMENT 16
#define ALIGN(size) (((size) + (ALIGNMENT - 1)) & ~(ALIGNMENT - 1))

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
