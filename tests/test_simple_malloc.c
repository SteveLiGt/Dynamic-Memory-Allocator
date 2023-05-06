#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define ALLOC_SIZE 48

/* This test checks to ensure that the program break does not move
 * between two allocations of the same size.  Note that this test WILL
 * PASS with the given allocator, but that the given allocator does NOT
 * meet the requirements of this assignment.  This test should, however,
 * help you debug your own allocator and think of ways to write your own
 * tests for your allocator. */
int main(int argc, char *argv[])
{
    void *p1, *p2, *brk1;

    /* First request an allocation of ALLOC_SIZE bytes to cause the
     * allocator to fetch some memory from the OS and "prime" this
     * allocation size block. */
    p1 = malloc(ALLOC_SIZE);
    //free(p1);
    /* Call sbrk(0) to retrieve the current program break without
     * changing it.  When we allocation again, this value should not
     * change, because the allocator should not fetch more memory from
     * the OS. */
    brk1 = sbrk(0);

    /* Request a second allocation of ALLOC_SIZE, which should simply
     * take an allocation block off the list for ALLOC_SIZE. */
    p2 = malloc(ALLOC_SIZE);
    //free(p2);
    /* This should never run -- but it causes p1 and p2 to be "used", so
     * the compiler doesn't fail. */
    if (brk1 == NULL) {
        fprintf(stderr, "sbrk() failed: %p %p\n", p1, p2);
    }

    /* Compare the new program break to the old and make sure that it
     * has not changed. */
    if (brk1 == sbrk(0)) {
        return 0;
    } else {
        return 1;
    }
}
