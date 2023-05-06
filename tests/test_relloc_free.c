#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define ALLOC_SIZE24 24
#define ALLOC_SIZE48 48
#define ALLOC_SIZE5000 5000
#define ALLOC_SIZE4089 4089


/* This test checks to ensure that the program break does not move
 * between two allocations of the same size.  Note that this test WILL
 * PASS with the given allocator, but that the given allocator does NOT
 * meet the requirements of this assignment.  This test should, however,
 * help you debug your own allocator and think of ways to write your own
 * tests for your allocator. */
int main(int argc, char *argv[])
{
    void *p1, *p2;
    
    /* First request an allocation of ALLOC_SIZE bytes to cause the
     * allocator to fetch some memory from the OS and "prime" this
     * allocation size block. */
    p1 = malloc(ALLOC_SIZE24);
    p1 -= sizeof(size_t);
    //fprintf(stderr, "malloc asking for 24 bytes, block size is %zu\n",*(size_t*)p1);
    p1 += sizeof(size_t);
    //fprintf(stderr, "calling realloc for block size 24 to 48\n");
    p1 = realloc(p1, ALLOC_SIZE48);
    p1 -= sizeof(size_t);
    //fprintf(stderr, "realloc asking for 48 bytes, block size is %zu\n",*(size_t*)p1);
    p1 += sizeof(size_t);


    p2 = malloc(ALLOC_SIZE4089);
    p2 -= sizeof(size_t);
    //fprintf(stderr, "malloc asking for 4089 bytes, block size is %zu\n",*(size_t*)p2);
    p2 += sizeof(size_t);
    //fprintf(stderr, "calling realloc for block size 4089 to 5000\n");
    p2 = realloc(p2, ALLOC_SIZE5000);
    p2 -= sizeof(size_t);
    //fprintf(stderr, "realloc asking for 5000 bytes, block size is %zu\n",*(size_t*)p2);
    p2 += sizeof(size_t);
    
    free(p2);
    
    return 0; 
  
   
}
