#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

void *bulk_alloc(size_t size);
void *bulk_free(void *ptr, size_t size);

#define ALLOC_SIZE 4096

/*
 * Note that it's not really safe to use stdio here, because that
 * predicates a working malloc(), which we're not guaranteed to have.
 * Therefore, we're going to use UNIX I/O with statically allocated
 * strings.
 */ 
int main(int argc, char *argv[]) {
    const char alloc_error[] = "bulk_alloc() failed\n";
    const char alignment_error[] = "bulk allocation not aligned\n";
    char *mem;

    if ((mem = bulk_alloc(ALLOC_SIZE)) == NULL) {
        write(1, alloc_error, sizeof(alloc_error));
        return 1;
    }

    if ((uintptr_t)mem % 8 != 0) {
        write(1, alignment_error, sizeof(alignment_error));
        return 2;
    }

    /* Exercise the memory we recieved to make sure it's mapped. */
    for (int i = 0; i < ALLOC_SIZE; i++) {
        mem[i] = i % 0xff;
    }

    bulk_free(mem, ALLOC_SIZE);

    return 0;
}
