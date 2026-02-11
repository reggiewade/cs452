#include "freelist.h"
#include "utils.h"

/**
 * @brief Creates a new free list for managing free blocks of memory in a buddy allocator.
 *
 * The free list is organized by block size, with each level corresponding to blocks of size 2^e.
 *
 * @param size Size of the memory pool.
 * @param l Lower bound of the exponent range.
 * @param u Upper bound of the exponent range.
 * @return FreeList Pointer to the newly created free list.
 */

extern FreeList freelistcreate(size_t size, int l, int u) {
    int i = u - l + 1;
    void **freeList = mmalloc(i * sizeof(void *));

    for (int j = 0; j < i; j++) {
        freeList[j] = NULL;
    }
    return (FreeList)freeList;
}

/**
 * @brief Deletes a free list, freeing all associated resources.
 * 
 *
 * @param f Pointer to the free list to be deleted.
 * @param l Lower bound of the exponent range.
 * @param u Upper bound of the exponent range.
 */

extern void freelistdelete(FreeList f, int l, int u) {
    int i = u - l + 1;
    mmfree(f, i * sizeof(void *));
}

/**
 * @brief Allocates a block of memory from the free list corresponding to the specified exponent level.
 *
 * The function will return a pointer to a free block of memory of size 2^e if available, or NULL if no such block is free.
 *
 * @param f Pointer to the free list.
 * @param base Base address of the memory pool.
 * @param e Exponent level of the block to be allocated.
 * @param l Lower bound of the exponent range.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 */

extern void *freelistalloc(FreeList f, void *base, int e, int l) {
    if (base != NULL) {
        void **freeList = (void **)f;
        int index = e - l;
        freeList[index] = *(void **)base;
        return base;
    }
    return NULL;
}

/**
 * @brief Frees a block of memory back to the free list corresponding to the specified exponent level.
 *
 * The function will add the block of memory back to the free list for blocks of size 2^e, making it available for future allocations.
 *
 * @param f Pointer to the free list.
 * @param base Base address of the memory pool.
 * @param mem Pointer to the block of memory to be freed.
 * @param e Exponent level of the block to be freed.
 * @param l Lower bound of the exponent range.
 */

extern void freelistfree(FreeList f, void *base, void *mem, int e, int l) {
    void **freeList = (void **)f;
    int index = e - l;
    if (base != NULL) {
        *(void **)mem = base;
        freeList[index] = mem;
        return;
    }
    *(void **)mem = NULL;
    freeList[index] = mem;
    return;
}

/**
 * @brief Gets the total number of free blocks across all levels in the free list.
 *
 * This function iterates through all levels of the free list and counts the total number of free blocks available.
 *
 * @param f Pointer to the free list.
 * @param base Base address of the memory pool.
 * @param mem Pointer to a block of memory (not used in counting).
 * @param e Exponent level (not used in counting).
 * @param l Lower bound of the exponent range.
 * @param u Upper bound of the exponent range.
 * @return The total number of free blocks in the free list.
 */

extern int freelistsize(FreeList f, void *base, void *mem, int l, int u) {
    void **freeList = (void **)f;
    int total = 0;
    for (int i = 0; i < u - l + 1; i++) {
        void *curr = freeList[i];
        while (curr != NULL) {
            total++;
            curr = *(void **)curr;
        }
    }
    return total;
}

/**
 * @brief Prints the contents of the free list for debugging purposes.
 *
 * This function iterates through each level of the free list and prints the addresses of the free blocks at each level.
 *
 * @param f Pointer to the free list.
 * @param l Lower bound of the exponent range.
 * @param u Upper bound of the exponent range.
 */

extern void freelistprint(FreeList f, int l, int u) {
    void **freeList = (void **)f;
    for (int i = 0; i < u - l + 1; i++) {
        printf("Level %d: ", l + i);
        void *curr = freeList[i];
        while (curr != NULL) {
            printf("%p -> ", curr);
            curr = *(void **)curr;
        }
        printf("NULL\n");
    }
}