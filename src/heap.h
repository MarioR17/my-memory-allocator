#include <stddef.h>
#include <stdbool.h>

struct ChunkHeader {
        size_t size;
        bool is_free;
};

typedef struct FreeListNode {
        struct FreeListNode *next;
        struct FreeListNode *prev;
} FreeNode;

FreeNode *free_list_head = NULL;

void *malloc(size_t num_bytes);
