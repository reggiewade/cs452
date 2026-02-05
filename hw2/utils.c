#include "utils.h"
#include <sys/mman.h>

extern void *mmalloc(size_t size) {
    void *mem = mmap(0,size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1,0);
    return (mem == MAP_FAILED) ? NULL : mem;
}

extern void mmfree(void *p, size_t size) {
    if (p) {
        munmap(p,size);
    }
    else {
        return NULL;
    }
}

extern size_t divup(size_t n, size_t d) {
    return (n + d - 1) / d;
}

extern size_t bits2bytes(size_t bits) {
    return divup(bits, bitsperbyte);
}

extern size_t e2size(int e) {
    return (size_t)1 << e;
}

extern int size2e(size_t size) {
    int e = 0;
    while ((size_t)1 << e < size) {
        e++;
    }
    return e;
}

extern void bitset(void *p, int bit) {
    ((unsigned char *)p)[bit / bitsperbyte] |= (1 << (bit % bitsperbyte));
}

extern void bitclr(void *p, int bit) {
    ((unsigned char *)p)[bit / bitsperbyte] &= ~(1 << (bit % bitsperbyte));
}

extern void bitinv(void *p, int bit) {
    ((unsigned char *)p)[bit / bitsperbyte] ^= (1 << (bit % bitsperbyte));
}

extern int bittst(void *p, int bit) {
    return (((unsigned char *)p)[bit / bitsperbyte] >> (bit % bitsperbyte)) & 1;
}