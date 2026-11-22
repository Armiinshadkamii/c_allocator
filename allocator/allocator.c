#include "allocator.h"

#include <sys/mman.h>
#include <stdint.h>

#define HEAP_SIZE (1024 * 1024)

struct block {
    size_t size;
    int is_free;
    block_t *next;
    max_align_t alignment;
};

// the beginning of our mapped memory.
static void *heap_start = NULL;
// the first block in our linked list
static block_t *head = NULL;

static int initialize_heap(void) {
    void *memory = mmap(
        NULL,
        HEAP_SIZE,
        PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS,
        -1,
        0
    );

    if (memory == MAP_FAILED) {
        return -1;
    }

    heap_start = memory;
    head = (block_t *)memory;
    head->size = HEAP_SIZE - sizeof(block_t);
    head->is_free = 1;
    head->next = NULL;

    return 0;
}

/**
 * @brief Finds the first free block in the linked list that can fit the requested size.
 *
 * Walks the block list starting at `head` and returns the first block marked
 * as free whose size is >= the requested size. This is a first-fit strategy;
 * the block is NOT removed from the list or split — the caller is responsible
 * for marking it used and/or splitting it if larger than needed.
 *
 * @param size Minimum number of bytes the returned block must accommodate.
 * @return Pointer to a suitable free block, or NULL if none is large enough.
 *
 * @note Not thread-safe: `head` and block flags are read without locking.
 */
static block_t *find_free_block(size_t size) {
    block_t *current = head;
    while (current != NULL) {
        if (current->size >= size && current->is_free) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}


static void split_block(block_t *block, size_t size) {
    if (block->size <= size + sizeof(block_t)) {return;}
    block_t *new_block = (block_t *)(
      (char *)block + sizeof(block_t) + size
    );
    new_block->size = block->size - size - sizeof(block_t);
    new_block->is_free = 1;
    new_block->next = block->next;

    block->size = size;
    block->next = new_block;
}

static size_t align_size(size_t size) {
    const size_t alignment = _Alignof(max_align_t);
    const size_t remainder = size % alignment;

    if (remainder == 0) {
        return size;
    }

    const size_t padding = alignment - remainder;

    if (size > SIZE_MAX - padding) {
        return 0;
    }

    return size + padding;
}

/**
 * @brief try's to find a free memory block, marks it as occupied, returns the address to the free block.
 * @param size Minimum number of bytes the returned block must accommodate.
 * @return free memory address
 */
void *my_malloc(size_t size) {
    size = align_size(size);
    if (size == 0) {
        return NULL;
    }

    if (head == NULL) {
        if (initialize_heap() == -1) {
            return NULL;
        }
    }
    // return (char *)block + sizeof(block_t);
    block_t *block = find_free_block(size);
    if (block == NULL) {
        return NULL;
    }

    split_block(block, size);
    block->is_free = 0;
    // sizeof(block_t) the number of bytes occupied by our metadata structure
    // block + 1 moves forward by sizeof(block_t) bytes. (char *)block moves
    // exactly one byte.
    // So the returned pointer points immediately after the metadata.
    // see the memory simulation comment on main.c
    return (char *)block + sizeof(block_t);
}

// when 2 blocks of memory dont have enough space
// to allocated individually, they can be combined.
static void coalesce_blocks(void) {
    block_t *current = head;

    while (current != NULL && current->next != NULL) {
        block_t *next = current->next;

        const char *current_end =
            (char *)current + sizeof(block_t) + current->size;

        // check if both blocks are free.
        if (current->is_free &&
            next->is_free &&
            current_end == (char *)next) { // Check that they are physically adjacent

            current->size += sizeof(block_t) + next->size; // Merge their sizes.
            current->next = next->next;

            } else {
                current = next;
            }
    }
}

void my_free(void *ptr) {
    if (ptr == NULL) {
        return;
    }

    block_t *block = (block_t *)(
        (char *)ptr - sizeof(block_t)
    );

    block->is_free = 1;
    coalesce_blocks();
}