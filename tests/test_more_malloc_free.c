#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define ALLOC_SIZE 24

/* This test checks to ensure that the program break does not move
 * between two allocations of the same size.  Note that this test WILL
 * PASS with the given allocator, but that the given allocator does NOT
 * meet the requirements of this assignment.  This test should, however,
 * help you debug your own allocator and think of ways to write your own
 * tests for your allocator. */
int main(int argc, char *argv[])
{
    void *p1,*p2, *p3, *p4, *brk1;

    /* First request an allocation of ALLOC_SIZE bytes to cause the
     * allocator to fetch some memory from the OS and "prime" this
     * allocation size block. */
    p1 = malloc(ALLOC_SIZE);
    brk1 = sbrk(0);

    p2 = malloc(ALLOC_SIZE);
    /* Compare the new program break to the old and make sure that it
     * has not changed. */
    if (brk1 == sbrk(0)) {
        return 0;
    } else {
        return 1;
    }
    p3 = malloc(ALLOC_SIZE);
    /* Compare the new program break to the old and make sure that it
     * has not changed. */
    if (brk1 == sbrk(0)) {
        return 0;
    } else {
        return 1;
    }
    p4 = malloc(ALLOC_SIZE);
    /* Compare the new program break to the old and make sure that it
     * has not changed. */
    if (brk1 == sbrk(0)) {
        return 0;
    } else {
        return 1;
    }
    
   


    
    fprintf(stderr, "\n calling free()for p1,p3,p2,p4. \n");
    /* Compare the new program break to the old and make sure that it
     * has not changed. */
    if (brk1 == sbrk(0)) {
        return 0;
    } else {
        return 1;
    }
    free(p1);
    free(p3);
    free(p2);
    free(p4);
    /* Compare the new program break to the old and make sure that it
     * has not changed. */
    if (brk1 == sbrk(0)) {
        return 0;
    } else {
        return 1;
    }
    return 0; 
}
