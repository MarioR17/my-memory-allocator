#include <stdint.h>

#include "heap.h"
#include "print.h"

FreeNode *free_list_head = NULL;

static void *os_mem_request(size_t num_bytes);
static int freeblock_available(size_t num_bytes);
static void *get_chunk_from_freelist(size_t num_bytes);
static void *split_chunk(struct ChunkHeader *chunk, size_t num_bytes);
static void remove_from_freelist(FreeNode *node);

/*
 * Remove a given node from the freelist and adjust the list accordingly.
 */
static void remove_from_freelist(FreeNode *node)
{
        if (!node) return;

        if (node->prev) {
                node->prev->next = node->next;
        } else {
                free_list_head = node->next;
        }

        if (node->next)
                node->next->prev = node->prev;

        node->next = NULL;
        node->prev = NULL;
}

/*
 * Take in a chunk and split it into a user chunk of num_bytes size
 * and a free chunk of the left over amount of bytes.
 *
 * Return the void pointer to the payload of the user chunk.
 */
static void *split_chunk(struct ChunkHeader *chunk, size_t num_bytes)
{
        void *user_payload;
        char *split_point;
        char *free_payload;
        struct ChunkHeader *user_block;
        struct ChunkHeader *free_block;
        FreeNode *new_node;
        size_t original_payload_size;

        original_payload_size = chunk->size;

        user_block = chunk;
        user_block->size = num_bytes;
        user_block->is_free = false;
        user_payload = (void*)((char*)chunk + HEADER_SIZE);

        // Where the free chunk starts
        split_point = (char*)chunk + HEADER_SIZE + user_block->size;

        free_block = (struct ChunkHeader*)split_point;
        free_block->size =  original_payload_size - HEADER_SIZE - user_block->size;
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
 * Iterate through the free list and select a chunk using the best fit algorithm.
 *
 * If there is space left over, split the chunk into the requested size and
 * the chunk left over.
 *
 * Update the free list to not include the user chunk and do include the 
 * left over chunk from the split that is free.
 *
 * Return a void pointer to the payload of the user requested chunk.
 */
static void *get_chunk_from_freelist(size_t num_bytes)
{
        FreeNode *best = NULL;
        FreeNode *curr = free_list_head;
        void *final_best_address;
        struct ChunkHeader *curr_chunk_header;
        struct ChunkHeader *final_best_header;

        while (curr) {
                void *curr_address = (void*)((char*)curr - HEADER_SIZE);
                curr_chunk_header = (struct ChunkHeader*)curr_address;

                if (curr_chunk_header->size < num_bytes) {
                        curr = curr->next;
                        continue;
                }

                if (curr_chunk_header->size == num_bytes) {
                        best = curr;
                        break;
                }

                if (best) {
                        void *best_address = (void*)((char*)best - HEADER_SIZE);
                        struct ChunkHeader *temp_best_header = 
                                (struct ChunkHeader*)best_address;

                        size_t best_diff = temp_best_header->size - num_bytes; 
                        size_t curr_diff = curr_chunk_header->size - num_bytes;

                        if (curr_diff < best_diff) best = curr;
                } else {
                        best = curr;
                }

                curr = curr->next;
        }

        if (!best) return NULL;

        remove_from_freelist(best);

        final_best_address = (void*)((char*)best - HEADER_SIZE);
        final_best_header = (struct ChunkHeader*)final_best_address;

        return split_chunk(final_best_header, num_bytes);
}

/*
 * Iterate through the free list, stopping and returning 1 when we see
 * a chunk that is at least as big as num_bytes.
 * 
 * Return 0 otherwise, indicating we don't have such a chunk right now.
 */
static int freeblock_available(size_t num_bytes)
{
        FreeNode *curr = free_list_head;
        struct ChunkHeader *curr_chunk_header;

        while (curr) {
                void *header_address = (void*)((char*)curr - HEADER_SIZE);
                curr_chunk_header = (struct ChunkHeader*)header_address;

                if (curr_chunk_header->size >= num_bytes) {
                        return 1;
                }

                curr = curr->next;
        }

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
static void *os_mem_request(size_t num_bytes)
{
        void *page_start; 

        page_start = sbrk(PAGE_SIZE);

        if (page_start == (void*)-1) {
                print_str("sbrk failed\n");
                return NULL;
        }

        struct ChunkHeader *new_page = (struct ChunkHeader*)page_start;
        new_page->size = PAGE_SIZE - HEADER_SIZE;
        new_page->is_free = true;

        return split_chunk(new_page, num_bytes); 
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

        if ((free_list_head == NULL) || 
        (freeblock_available(aligned_payload_size) == 0)) {
                mem_ptr = os_mem_request(aligned_payload_size);
                if (mem_ptr) print_str("Got memory from new heap increase\n");
        } else {
                mem_ptr = get_chunk_from_freelist(aligned_payload_size);
                if (mem_ptr) print_str("Got memory from free list chunk\n");
        }

        return mem_ptr;
}

/*
 * Return the user with heap memory of the requested size just like malloc
 * except this time each byte is zero'd out guaranteed by the time the user
 * receives the pointer to the memory
 */
void *calloc(size_t num_items, size_t items_size)
{
        if (num_items == 0 || items_size == 0) return NULL;
        if (items_size > SIZE_MAX / num_items) return NULL;

        void *user_ptr;
        char *byte_array;
        size_t total_aligned_size;

        total_aligned_size = ALIGN(num_items * items_size);

        user_ptr = malloc(total_aligned_size);

        if (user_ptr == NULL) return NULL;

        byte_array = (char*)user_ptr;

        for (size_t i = 0; i < total_aligned_size; i++) {
                byte_array[i] = 0;
        }

        return user_ptr;
}

/*
 * Free the given heap memory by marking it free and putting it
 * into the freelist or do nothing if given a NULL pointer.
 */
void free(void *ptr)
{
        if (!ptr) return;

        struct ChunkHeader *header;
        FreeNode *new_node;

        header = (struct ChunkHeader*)((char*)ptr - HEADER_SIZE);
        header->is_free = true;

        new_node = (FreeNode *)ptr;
        new_node->next = free_list_head;
        new_node->prev = NULL;

        if (free_list_head)
                free_list_head->prev = new_node;

        free_list_head = new_node;
}
