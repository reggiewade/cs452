#include "balloc.h"
#include "freelist.h"
#include "bbm.h"
#include "utils.h"

typedef struct {
    unsigned int size;
    int l;
    int u;
    FreeList f;
    void* base;
    BBM* bitmap;
} BallocStruct;

/**
 * @brief Allocates a new buddy memory pool of the specified size and exponent range.
 *
 * The size must be a power of two, and the exponent range [l, u] must satisfy 0 <= l <= u and 2^u <= size.
 *
 * @param size Size of the memory pool.
 * @param l Lower bound of the exponent range.
 * @param u Upper bound of the exponent range.
 * @return Balloc Pointer to the newly created buddy memory pool.
 */
extern Balloc bcreate(unsigned int size, int l, int u) {
    BallocStruct* b = (BallocStruct*)mmalloc(sizeof(BallocStruct));
    b->size = size;
    b->l = l;
    b->u = u;
    b->f = freelistcreate(size, l, u);
    b->base = mmalloc(size);

    if (!b->f) {
        mmfree(b, sizeof(BallocStruct));
        return NULL;
    }

    b->bitmap = (BBM*)mmalloc((u - l + 1) * sizeof(BBM));
    for (int i = l; i <= u; i++) {
        b->bitmap[i - l] = bbmcreate(size, i);
    }

    freelistfree(b->f, NULL, b->base, b->u, b->l);
    return (Balloc) b;
}

/**
 * @brief Deletes a buddy memory pool, freeing all associated resources.
 *
 * This function will free the memory pool, the bitmaps, and the free lists associated with the buddy allocator.
 *
 * @param pool Pointer to the memory pool to be deleted.
 */
extern void bdelete(Balloc pool) {
    BallocStruct* b = (BallocStruct*)pool;
    for (int i = b->l; i <= b->u; i++) {
        bbmdelete(b->bitmap[i - b->l]);
    }
    mmfree(b->bitmap, (b->u - b->l + 1) * sizeof(BBM));
    freelistdelete(b->f, b->l, b->u);
    mmfree(b->base, b->size);
    mmfree(b, sizeof(BallocStruct));
}

/**
 * @brief Allocates a block of memory of the specified size from the provided buddy memory pool.
 *
 * 
 * @param pool Pointer to the buddy memory pool.
 * @param size Size of the memory block to be allocated.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 */
extern void *balloc(Balloc pool, unsigned int size) {
    BallocStruct* b = (BallocStruct*)pool;
    int e = size2e(size);
    if (e < b->l) return NULL;
    if (e > b->u) e = b->u;
    void** list = (void**)b->f;

    // initial check for free block at the desired level
    if (list[e - b->l] != NULL) {
        return freelistalloc(b->f, list[e - b->l], e, b->l);
    }

    // find larger block to split
    for (int i = e + 1; i <= b->u; i++) {
        if (list[i - b->l] != NULL) {
            void* mem = freelistalloc(b->f, list[i - b->l], i, b->l);
            bbmset(b->bitmap[i - b->l], b->base, mem, i);
            for (int j = i - 1; j >= e; j--) {
                void* buddy = baddrinv(b->base, mem, j);
                freelistfree(b->f, list[j - b->l], buddy, j, b->l);
                bbmset(b->bitmap[j - b->l], b->base, mem, j);
            }
            return mem;
        }
    }
    return NULL;
}

/**
 * @brief Frees a previously allocated block of memory back to the buddy memory pool.
 *
 *
 * @param pool Pointer to the buddy memory pool.
 * @param mem Pointer to the memory block to be freed.
 */
extern void bfree(Balloc pool, void *mem) {
    BallocStruct* b = (BallocStruct*)pool;
    void** list = (void**)b->f;
    
    unsigned int s = bsize(pool, mem);
    if (s == 0) return;
    size_t e = size2e(s);

    // Try to coalesce upwards 
    for (int i = e; i < b->u; i++) {
        void* buddy = baddrinv(b->base, mem, i);
        int idx = i - b->l;

        // INLINE SEARCH: Check if buddy is in the free list at this level.
        // We traverse the linked list manually.
        void* curr = list[idx];
        void* prev = NULL;
        int found = 0;

        while (curr != NULL) {
            if (curr == buddy) {
                // Found the buddy! Remove it from the list.
                // The list is singly linked; 'curr' points to the next node.
                void* next = *(void**)curr; 
                
                if (prev == NULL) {
                    list[idx] = next; // Update head
                } else {
                    *(void**)prev = next; // Update prev's next
                }
                found = 1;
                break;
            }
            prev = curr;
            curr = *(void**)curr;
        }

        if (found) {
            // Buddy was free and we removed it. Merge!
            // Mark this level as UNSPLIT (0)
            bbmclr(b->bitmap[idx], b->base, mem, i);

            // Move up to the next level
            mem = baddrclr(b->base, mem, i);
            
            // Continue loop to try merging at i+1
        } else {
            // Buddy is not free. Stop merging.
            // Add current block to free list and return.
            freelistfree(b->f, list[idx], mem, i, b->l);
            return;
        }
    }

    // If we merge all the way to the top (u), add it to the u-list
    freelistfree(b->f, list[b->u - b->l], mem, b->u, b->l);
}

/**
 * @brief Allocates a new buddy memory pool of the specified size and exponent range.
 *
 * The size must be a power of two, and the exponent range [l, u] must satisfy 0 <= l <= u and 2^u <= size.
 *
 * @param size Size of the memory pool.
 * @param l Lower bound of the exponent range.
 * @param u Upper bound of the exponent range.
 * @return Balloc Pointer to the newly created buddy memory pool.
 */
extern unsigned int bsize(Balloc pool, void *mem) {
    BallocStruct* b = (BallocStruct*)pool;
    for (int i = b->l; i <= b->u; i++) {
        if (bbmtst(b->bitmap[i - b->l], b->base, mem, i)) {
            return e2size(i);
        }
    }
    return e2size(b->u);
}

extern void bprint(Balloc pool) {
    BallocStruct* b = (BallocStruct*)pool;
    for (int i = b->l; i <= b->u; i++) {
        printf("Level %d: ", i);
        bbmprt(b->bitmap[i - b->l]);
    }
    freelistprint(b->f, b->l, b->u);
}