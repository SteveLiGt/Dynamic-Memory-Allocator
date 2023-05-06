
#include <stdlib.h>
#include <stdio.h>
typedef struct header_node{
    size_t size;
    struct header_node *next; 
}header_node;
int main()
{
    header_node * new_node = malloc(sizeof(header_node));
    void * start = sbrk(4096);
    header_node * node = (header_node*)start;
    /*
    node->size = 32;
    node->next = new_node;
    */
    *(size_t*)start = 64; 
    start += sizeof(size_t);
    // (header_node*)start = new_node;
    ((header_node*)start)->next = new_node;
    
    printf("address of start = %p\n", start);
    printf("size in my node is %zu\n", node->size);
    printf("size in my node is %p\n", start);
    printf("next ptr in my node is %p\n", node->next);
    printf("next ptr in my node is %p\n", new_node);
    printf("next ptr in my node is %p\n", (header_node*)start);
    return 0;
}
