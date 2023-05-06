#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>


void *bulk_alloc(size_t size) {
    void *mapping =  mmap(NULL, size, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (mapping == MAP_FAILED) {
        return NULL;
    } else {
        return mapping;
    }
}

void bulk_free(void *ptr, size_t size) {
    if (munmap(ptr, size)) {
        fprintf(stderr, "munmap failed in bulk_free(); you probably passed invalid arguments\n");
    }
}
