#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#define ALLOC_SIZE24 24
#define ALLOC_SIZE48 48
#define ALLOC_SIZE4088 4088


/* This test checks to ensure that the program break does not move
 * between two allocations of the same size.  Note that this test WILL
 * PASS with the given allocator, but that the given allocator does NOT
 * meet the requirements of this assignment.  This test should, however,
 * help you debug your own allocator and think of ways to write your own
 * tests for your allocator. */
int main(int argc, char *argv[])
{
    void *p1, *p2, *p3, *brk1, *brk2, *brk3;

    /* First request an allocation of ALLOC_SIZE bytes to cause the
     * allocator to fetch some memory from the OS and "prime" this
     * allocation size block. */
    p1 = malloc(ALLOC_SIZE24);
    free(p1);
    /* Call sbrk(0) to retrieve the current program break without
     * changing it.  When we allocation again, this value should not
     * change, because the allocator should not fetch more memory from
     * the OS. */
    brk1 = sbrk(0);

    /* Request a second allocation of ALLOC_SIZE, which should simply
     * take an allocation block off the list for ALLOC_SIZE. */
    p2 = malloc(ALLOC_SIZE48);
    //free(p2);
    free(p2);
    brk2 = sbrk(0);
   


    
    /* Request a second allocation of ALLOC_SIZE, which should simply
     * take an allocation block off the list for ALLOC_SIZE. */
    p3 = malloc(ALLOC_SIZE4088);
    //free(p2);
    free(p3);
    brk3 = sbrk(0);
    
    

     // check brk 1 and brk2 and brk3, they should all be different
     if (brk1 != brk2 && brk2 != brk3 &&  brk1 != brk3) {
  
        return 0;
    } else {
        fprintf(stderr, "brk1, brk2, brk3 should all be different");
        return 1;
    }
     


    
  
   
}
