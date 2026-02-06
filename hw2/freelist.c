#include "freelist.h"
#include "utils.h"


extern FreeList freelistcreate(size_t size, int l, int u) {
    int i = l - u + 1;
    void **freeList = mmalloc(i * sizeof(void *));

    for (int j = 0; j < i; j++) {
        freeList[j] = NULL;
    }
    return (FreeList)freeList;
}

extern void freelistdelete(FreeList f, int l, int u) {
    int i = l - u + 1;
    mmfree(f, i * sizeof(void *));
}

extern void *freelistalloc(FreeList f, void *base, int e, int l) {
    if (base != NULL) {
        void **freeList = (void **)f;
        int index = l - e;
        freeList[index] = *(void **)base;
        return base;
    }
    return NULL;
}

extern void freelistfree(FreeList f, void *base, void *mem, int e, int l) {
    void **freeList = (void **)f;
    int index = l - e;
    if (base != NULL) {
        *(void **)mem = base;
        freeList[index] = mem;
        return;
    }
    *(void **)mem = NULL;
    freeList[index] = mem;
    return;
}

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