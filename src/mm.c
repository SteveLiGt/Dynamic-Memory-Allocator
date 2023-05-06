
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
/* always recall that a block give by user will be something look like
**header**|****userdata****
         ptr 
and our header will always be 8 bytes which contain a 'size_t' variable 'size'
*/

/* When requesting memory from the OS using sbrk(), request it in
 * increments of CHUNK_SIZE. */
#define CHUNK_SIZE (1<<12)
#define TOTAL_INDEX 13  // 0 to 12
#define BLOCK_INDEX 5 // only used once in the freelist_table_initialize
#define SIZE_T_BYTES 8
// Struct , used for metadata, contain 'size' and 'next ptr'
typedef struct header_node{
    size_t size;
    struct header_node *next; 
}header_node;
//contain our free list table
static header_node **freelist_table = NULL;
//static size_t bulk_size = 0;
/* The standard allocator interface from stdlib.h.  These are the
 * functions you must implement, more information on each function is
 * found below. They are declared here in case you want to use one
 * function in the implementation of another. */
void *malloc(size_t size);
void free(void *ptr);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);
//static int  freelist_validate(int freelist_index);// call in the test
/* my helper function */
static void set_free(void *); // set the last bit of 'size' in the header to '0', indicate freed
static void set_alloc(void *);// set the last bit of 'size' in the header to '1', indicate allocated
static void freelist_table_initialize(); // initialize the freelist_table
static void freelist_blocks(void * , int index, int); // create (1<<int) blocks for freelist_table[index]
static void* get_block(int index); // return block for require size, by removing a Block from freelist_table[index]
static void add_block(void * ptr); // received user Ptr, and add it back to free list. (It need to add NextPtr into it and connectit with next non-NULL header_node )
//static bool freelist_validate(); //this function will have access directly to my freelist_table, and be able to check my freelist from there.






/*
 * This function, defined in bulk.c, allocates a contiguous memory
 * region of at least size bytes.  It MAY NOT BE USED as the allocator
 * for pool-allocated regions.  Memory allocated using bulk_alloc()
 * must be freed by bulk_free().
 
 * This function will return NULL on failure.
 */
extern void *bulk_alloc(size_t size);

/*
 * This function is also defined in bulk.c, and it frees an allocation
 * created with bulk_alloc().  Note that the pointer passed to this
 * function MUST have been returned by bulk_alloc(), and the size MUST
 * be the same as the size passed to bulk_alloc() when that memory was
 * allocated.  Any other usage is likely to fail, and may crash your
 * program.
 */
extern void bulk_free(void *ptr, size_t size);

/*
 * This function computes the log base 2 of the allocation block size
 * for a given allocation.  To find the allocation block size from the
 * result of this function, use 1 << block_size(x).
 *
 * Note that its results are NOT meaningful for any
 * size > 4088!
 *
 * You do NOT need to understand how this function works.  If you are
 * curious, see the gcc info page and search for __builtin_clz; it
 * basically counts the number of leading binary zeroes in the value
 * passed as its argument.
 */
static inline __attribute__((unused)) int block_index(size_t x) {
    if (x <= 8) {
        return 5;
    } else {
        return 32 - __builtin_clz((unsigned int)x + 7);
    }
}

/*
 * You must implement malloc().  Your implementation of malloc() must be
 * the multi-pool allocator described in the project handout.
 */
void *malloc(size_t size) {
    // if size = 0, then we return NULL
    if(size <= 0 ){
        return NULL; 
        
    }
        
    // check the size if it is more than 4088, we call bulk_alloc()
    if(size > 4088)
    {
    
    // not sure if I need to initialize the header
    // or the bulk_alloc() will initialize it for me
    //block += sizeof(size_t);
        void * block = bulk_alloc(size+ SIZE_T_BYTES);
        ((header_node*)block)->size = (size+SIZE_T_BYTES);
        set_alloc(block);// set the flag bit for debug maybe
        block += sizeof(size_t);//move over the header 
        return block;
        
    }
    
    //now the size must be 1 - 4088
    //get the required index size first
    
    /*###################################### 
    int freelist_index = block_index(size);
    //add some debug code

    fprintf(stderr, "\nin malloc, given size = %zu, freelist_index = %d\n",size,freelist_index);
    ###################################### */
    int freelist_index = block_index(size);
    if(freelist_table == NULL)
        //we need to create the free list first
        freelist_table_initialize();
    if(freelist_table[freelist_index] == NULL){
        void *start = sbrk(CHUNK_SIZE);
        if(start == (void*)-1){
            //fprintf(stderr, "sbrk() failed");
            return NULL;
        }
        freelist_table[freelist_index] = start;// make connection for the freelist_table 
        freelist_blocks(start, freelist_index, CHUNK_SIZE);
        
            
    }
   
    // I need to cast
   
    return get_block(freelist_index); 

    
    //return bulk_alloc(size);
}

/*
 * You must also implement calloc().  It should create allocations
 * compatible with those created by malloc().  In particular, any
 * allocations of a total size <= 4088 bytes must be pool allocated,
 * while larger allocations must use the bulk allocator.
 *
 * calloc() (see man 3 calloc) returns a cleared allocation large enough
 * to hold nmemb elements of size size.  It is cleared by setting every
 * byte of the allocation to 0.  You should use the function memset()
 * for this (see man 3 memset).
 */
void *calloc(size_t nmemb, size_t size) {
    if(nmemb == 0 || size == 0)
        return NULL;
    //now suppose it is a valid
    
    size_t total_bytes = nmemb * size;
    void * block = malloc(total_bytes);
   
    //For calloc, we want to set the total_bytes to '0'
    
    memset(block, 0, total_bytes);
    
    return block; 

}

/*
 * You must also implement realloc().  It should create allocations
 * compatible with those created by malloc(), honoring the pool
 * alocation and bulk allocation rules.  It must move data from the
 * previously-allocated block to the newly-allocated block if it cannot
 * resize the given block directly.  See man 3 realloc for more
 * information on what this means.
 *
 * It is not possible to implement realloc() using bulk_alloc() without
 * additional metadata, so the given code is NOT a working
 * implementation!
 */
void *realloc(void *ptr, size_t size) {
    //exclude some edge cases
    if(ptr == NULL){
        // malloc() will return NULL if failed
        // return valid 'ptr' on success
        return malloc(size); 
    }
    if(size == 0){
        // as suggested in handout, if size == 0
        // just return NULL 
        return NULL;
    }
    //now suppose ptr is vaild, and size != 0     
    //there are 2 major cases here
    /*###########case 1############*/
    //if the 'size' in block is bigger than required 'size'
    //we should return 'ptr' directly
    
    ptr -= sizeof(size_t); //move back 8 bytes to header
    size_t old_block_size = ((header_node*)ptr)->size;
    //the actual block size should be count without
    //'size_t' bytes + 'flag' byte = 8 + 1 = 9 
    size_t user_data_size = (old_block_size - 9);
    
    if( user_data_size>= size){
        
        ptr += sizeof(size_t);//move on 8 bytes to 'user data'
        return ptr;
        
    }
    else{ // actual_size < size
    /*###########case 2############*/    
    //if the 'size' in the block is lesser than required 'size'
    //we need to find a new 'bigger' block for 'size'
    //copy the old existing block user_data to new block
    // free(old block)
    //return 'new block'
       
        void * new_block = malloc(size);
        memcpy(new_block, ptr, user_data_size);
        free(ptr);
        return new_block;
        
    }  
    
}

/*
 * You should implement a free() that can successfully free a region of
 * memory allocated by any of the above allocation routines, whether it
 * is a pool- or bulk-allocated region.
 *
 * The given implementation does nothing.
 */
void free(void *ptr) {
    if(ptr == NULL)
        return;
    // so suppose the Ptr is valid
    // move back for size_t bytes or 8 bytes
    // call set_free() to set the size to be free
    // compare the result size with 4096
    ptr -= sizeof(size_t); // make ptr point to header
    set_free( ptr ); // it will set the  last bit of  header 'size'
    
    size_t size = ((header_node*)ptr)->size;
    
    if(size > CHUNK_SIZE){ // CHUNK_SIZE = 4096 bytes

        bulk_free(ptr, size);// size may need to -8
        return;
    }
    // use add_block_size(ptr) to add this ptr back to our free list
   
    add_block(ptr);
    //
    return;
}

/*############### helper functions here#################### */

 // set the last bit of 'size' in the header to '0', indicate freed
static void set_free(void * ptr_header){
    /*############################################ */

    /*############################################ */
    size_t block_size = ((header_node*)ptr_header)->size;
    block_size = block_size & ~0x1; // set the last bit to '0'
    ((header_node*)ptr_header)->size = block_size;
    /*############################################ */

    /*############################################ */
    return;
    
}
// set the last bit of 'size' in the header to '1', indicate allocated
static void set_alloc(void *ptr_header){
    size_t block_size = ((header_node*)ptr_header)->size;
    block_size = block_size | 0x1; // set the last bit to '1'
   ((header_node*)ptr_header)->size = block_size;
    return;

}
// initialize the freelist_table
static void freelist_table_initialize()
{
    // using the index of **freelist_table
    void * start = sbrk(CHUNK_SIZE);
    freelist_table = start;
    for(int i = 0; i < TOTAL_INDEX; i++){
        
        freelist_table[i] = NULL;
        start += sizeof(header_node*);
       
    }
    /*###make use of unused 3992 bytes###
    // make use of rest of 4096 bytes by calling create_blocks() to divide it  into 32 bytes of blocks.
    freelist_blocks(start, BLOCK_SIZE);
    // for right now, I ignore the rest of memory
    */
    int unused_bytes = CHUNK_SIZE - (13 * sizeof(header_node*));
    freelist_blocks(start, BLOCK_INDEX, unused_bytes);
    
    
    
    
}
/*
// create_blocks() will be used for calling sbrk() for creating blocks that could be added into freelist,
which willl be connected with freelist_table[], and then being later could be used by users by calling malloc();
// return -1 if sbrk() falied, otherwise create (1<<int) blocks for freelist_table[index]
*/
static void freelist_blocks(void * start_position, int index,int available_bytes){
    /*
    // we need to evenly divide the 4096 bytes into block_number
    of blocks, each block has size block_size.
    */
    //Now we assume we have Memory we needed

   
    size_t block_size = (1<<index);// 1<<5 = 2^5 = 32
    int block_num = available_bytes/block_size ;// 1<<(12-5) = 4096/32 = 128
    block_num -= 1; 
    // freelist_table[index] = first block
    header_node *block = NULL;
    for(int i = 0; i< block_num; i++){
        block = (header_node*)start_position;
        block->size = block_size;
        start_position += (block_size * sizeof(char));
        block->next = (header_node*)start_position;
        //start_position += block_size * sizeof(char);

        
    }
    block = (header_node*)start_position;
    block->size = block_size;
    block->next = NULL;


    return; // success
}
// return block for require size, by removing a Block from freelist_table[index], also, update the connection
static void* get_block(int index){
    
    header_node * block = freelist_table[index];
    void * set_block_alloc = block;
    set_alloc(set_block_alloc);// set 'size' last one bit to '1'
    freelist_table[index] = block->next; // update freelist
    set_block_alloc += sizeof(size_t);// move over first size_t bytes to user data area
   

    return set_block_alloc; // give it to user
}
; // received user Ptr, and add it back to free list. (It need to add NextPtr into it and connectit with next non-NULL header_node )
static void add_block(void * ptr){
    
    
    int index = block_index( ((header_node*)ptr)->size - SIZE_T_BYTES );
    
    header_node *next_block = freelist_table[index];
    header_node *block = (header_node*)ptr;
    // set the block header variables
    //block->size =((header_node*)ptr)->size;
    block->next = next_block;
    freelist_table[index] = block;
    
}


