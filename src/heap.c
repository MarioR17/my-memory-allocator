#include "heap.h"
#include "print.h"

FreeNode *free_list_head = NULL;

/*
 * Iterate through the free list, stopping and returning 1 when we see
 * a chunk that is at least as big as num_bytes.
 * 
 * Return 0 otherwise, indicating we don't have such a chunk right now.
 */
int freeblock_available(size_t num_bytes)
{
        FreeNode *curr = free_list_head;
        struct ChunkHeader *curr_chunk_header;

        while (curr) {
                void *header_address = (void*)((char*)curr - HEADER_SIZE);
                curr_chunk_header = (struct ChunkHeader*)header_address;

                print_str("Size of current chunk looking at: ");
                print_szt(curr_chunk_header->size);
                print_str("\n");

                if (curr_chunk_header->size >= num_bytes) {
                        print_str("Found a suitable chunk. Stopping with success.\n");
                        return 1;
                }
        }

        print_str("Did not find a suitable chunk. Stopping with failure.\n");

        return 0;
}

/*
 * Increase the break pointer of the process' heap with the sbrk() syscall.
 *
 * Split the new page of memory into two chunks one for the user's
 * chunk with the header and payload and one for the rest of the memory,
 * also with its header, which will be added to the free list.
 *
 * Return a void pointer to the newly created user's payload 
 * or a null pointer on failure from sbrk().
 */
void *os_mem_request(size_t num_bytes)
{
        void *page_start; 
        void *user_payload;
        char *split_point;
        char *free_payload;
        struct ChunkHeader *user_block;
        struct ChunkHeader *free_block;
        FreeNode *new_node;

        page_start = sbrk(PAGE_SIZE);

        if (page_start == (void*)-1) {
                print_str("sbrk failed\n");
                return NULL;
        }

        user_block = (struct ChunkHeader*)page_start;
        user_block->size = num_bytes;
        user_block->is_free = false;
        user_payload = (void*)((char*)page_start + HEADER_SIZE);

        // Where the free chunk starts
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

        return user_payload;
}

/*
 * Return the user with a pointer to heap memory of requested size.
 *
 * Could be newly acquired from the OS, or previously used memory that was 
 * freed and stored in the free list for future delegation.
 *
 * Return that pointer on success and a NULL pointer on failure.
 */
void *malloc(size_t num_bytes)
{
        if (num_bytes == 0) return NULL;

        void *mem_ptr;
        size_t aligned_payload_size = ALIGN(num_bytes);
        size_t total_block_size = aligned_payload_size + HEADER_SIZE;

        print_str("Total block size: ");
        print_szt(total_block_size);
        print_str("\n");

        if ((free_list_head == NULL) || (freeblock_available(num_bytes) == 0)) {
                mem_ptr = os_mem_request(total_block_size);
        } else {
                print_str("We have space available!!!!!\n");
                mem_ptr = NULL;
        }

        print_str("Address of memory pointer: ");
        print_ptr(mem_ptr);
        print_str("\n");

        return mem_ptr;
}
