#include "heap.h"
#include "print.h"

FreeNode *free_list_head = NULL;

int freeblock_available(size_t num_bytes)
{
        //FreeNode *curr = free_list_head;
        num_bytes++;

        return 0;
}

void *os_mem_request(size_t num_bytes)
{
        void *page_start; 
        struct ChunkHeader *user_block;
        struct ChunkHeader *free_block;
        char *split_point;
        char *free_payload;
        void *user_payload;
        FreeNode *new_node;

        page_start = sbrk(PAGE_SIZE);

        if (page_start == (void*)-1) {
                print_str("sbrk failed\n");
                return (void*)-1;
        }

        user_block = (struct ChunkHeader*)page_start;
        user_block->size = num_bytes;
        user_block->is_free = false;

        split_point = (char*)page_start + user_block->size;

        free_block = (struct ChunkHeader*)split_point;
        free_block->size = PAGE_SIZE - user_block->size;
        free_block->is_free = true;

        free_payload = split_point + HEADER_SIZE;

        new_node = (FreeNode*)free_payload;
        new_node->next = free_list_head;
        new_node->prev = NULL;

        if (free_list_head)
                free_list_head->prev = new_node;

        free_list_head = new_node;

        user_payload = (void*)((char*)page_start + HEADER_SIZE);

        return user_payload;
}

void *malloc(size_t num_bytes)
{
        if (num_bytes == 0) return NULL;

        size_t aligned_payload_size = ALIGN(num_bytes);
        size_t total_block_size = aligned_payload_size + HEADER_SIZE;
        void *mem_ptr;

        print_str("Total block size: ");
        print_szt(total_block_size);
        print_str("\n");

        if (free_list_head == NULL) {
                mem_ptr = os_mem_request(total_block_size);
        } else {
                mem_ptr = (void*)-1;
        }

        print_str("Address of memory pointer: ");
        print_ptr(mem_ptr);
        print_str("\n");

        return mem_ptr;
}
