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
    void** list = (void**)b->f;
    if (list[e] != NULL) {
        void* mem = freelistalloc(b->f, list[e], e, b->l);
        bbmset(b, b->base, mem, e);
        return mem;
    }
    for (int i = e + 1; i <= b->u; i++) {
        if (list[i] != NULL) {
            void* mem = freelistalloc(b->f, list[i], i, b->l);
            bbmset(b->bitmap, b->base, mem, i);
            for (int j = i - 1; j >= e; j--) {
                // flips the bit (e) to find the buddy
                void* buddy = baddrinv(b->base, mem, j);
                // adds the buddy block to the free list
                freelistfree(b->f, list[j], buddy, j, b->l);
                // marks the buddy block as free in the bitmap
                bbmclr(b->bitmap, b->base, buddy, j);
            }
            return mem;
        }
    }
    return NULL;
}

extern void bfree(Balloc pool, void *mem) {
    BallocStruct* b = (BallocStruct*)pool;
    void** list = (void**)b->f;
    size_t e = size2e(bsize(pool, mem));
    freelistfree(b->f, list[e], mem, e, b->l);
    bbmclr(b, b->base, mem, e);
    
    for (int i = e; i < b->u; i++) {
        void* buddy = baddrinv(b->base, mem, i);
        if (bbmtst(b->bitmap, b->base, buddy, i) == 0) {
            freelistfree(b->f, list[i], buddy, i, b->l);
            bbmclr(b->bitmap, b->base, buddy, i);
            mem = freelistalloc(b->f, list[i+1], i+1, b->l);
            bbmset(b->bitmap, b->base, mem, i+1);
            mem = (mem < buddy) ? mem : buddy;
        } else {
            break;
        }
    }
}

extern unsigned int bsize(Balloc pool, void *mem) {
    BallocStruct* b = (BallocStruct*)pool;
    for (int i = b->l; i <= b->u; i++) {
        if (bbmtst(b->bitmap, b->base, mem, i)) {
            return e2size(i);
        }
    }
    return 0;
}

extern void bprint(Balloc pool) {
    BallocStruct* b = (BallocStruct*)pool;
    for (int i = b->l; i <= b->u; i++) {
        printf("Level %d: ", i);
        bbmprt(b->bitmap[i - b->l]);
    }
    freelistprint(b->f, b->l, b->u);
}