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

    // mark the entire memory as free in the bitmap
    void** list = (void**)b->f;
    freelistfree(b->f, NULL, b->base, b->u, b->l);
    return (Balloc) b;
}

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

extern void *balloc(Balloc pool, unsigned int size) {
    BallocStruct* b = (BallocStruct*)pool;
    size_t e = size2e(size);
    if (e < b->l || e > b->u) return NULL;
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
            for (int j = i - 1; j >= (int)e; j--) {
                size_t jidx = j - b->l;
                void* buddy = baddrinv(b->base, mem, j);
                freelistfree(b->f, list[jidx], buddy, j, b->l);
                bbmset(b->bitmap[jidx], b->base, mem, j);
            }
            return mem;
        }
    }
    return NULL;
}

extern void bfree(Balloc pool, void *mem) {
    BallocStruct* b = (BallocStruct*)pool;
    void** list = (void**)b->f;
    unsigned int s = bsize(pool, mem);
    if (s == 0) return;
    size_t e = size2e(s);
    
    for (int i = e; i < b->u; i++) {
        void* buddy = baddrinv(b->base, mem, i);
        int idx = i - b->l;
        void* curr = list[idx];
        void* prev = NULL;
        int found = 0;

        while (curr != NULL) {
            if (curr == buddy) {
                void* next = *(void**)curr;

                if (prev == NULL) {
                    list[idx] = next;
                } else {
                    *(void**)prev = next;
                }
                found = 1;
                break;
            }
            prev = curr;
            curr = *(void**)curr;
        }
        if (found) {
            bbmclr(b->bitmap[idx], b->base, mem, i);
            mem = (mem < buddy) ? mem : buddy;
        }
        else {
            freelistfree(b->f, list[e - b->l], mem, e, b->l);
            return;
        }
    }
    freelistfree(b->f, list[b->u - b->l], mem, b->u, b->l);
}

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