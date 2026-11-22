// This is the architecture of our custom memory allocator.

/*
+----------------------+----------------------+
|      block_t         |      User memory     |
+----------------------+----------------------+
^                      ^
block                  returned pointer
 */
#include "allocator/allocator.h"

#include <stdio.h>
#include <string.h>

int main(void) {
    char *a = my_malloc(100);
    char *b = my_malloc(200);
    char *c = my_malloc(300);
    print_heap();

    if (!a || !b || !c) {
        printf("Allocation failed\n");
        return 1;
    }

    strcpy(a, "Hello from my allocator!");
    strcpy(b, "Memory management in C");

    printf("A: %s\n", a);
    printf("B: %s\n", b);

    my_free(b);
    my_free(a);
    print_heap();

    char *d = my_malloc(250);
    print_heap();

    if (d) {
        printf("D: Allocation successful!\n");
    }

    my_free(c);
    my_free(d);

    return 0;
}
