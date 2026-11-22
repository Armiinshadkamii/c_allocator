#ifndef C_ALLOCATOR_ALLOCATOR_H
#define C_ALLOCATOR_ALLOCATOR_H
#include <stddef.h>

typedef struct block block_t;

void *my_malloc(size_t size);
void my_free(void *ptr);
void print_heap(void);

#endif //C_ALLOCATOR_ALLOCATOR_H
