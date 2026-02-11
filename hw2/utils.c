#include "utils.h"
#include <sys/mman.h>

/**
 * @brief Allocates a block of memory with a specified size using memory mapping.
 *
 *
 * @param size Size of the memory block to be allocated.
 * @return Pointer to the allocated memory block, or NULL if allocation fails.
 */

extern void *mmalloc(size_t size) {
    void *mem = mmap(0,size, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1,0);
    return (mem == MAP_FAILED) ? NULL : mem;
}

/**
 * @brief Frees a block of memory back to the system using memory unmapping.
 *
 * @param p Pointer to the block of memory to be freed.
 * @param size Size of the memory block to be freed.
 */

extern void mmfree(void *p, size_t size) {
    if (p) {
        munmap(p,size);
    }
}

/**
 * @brief Performs integer division of n by d, rounding up to the nearest whole number.
 *
 *
 * @param n Numerator.
 * @param d Denominator.
 * @return The result of the division, rounded up to the nearest whole number.
 */

extern size_t divup(size_t n, size_t d) {
    return (n + d - 1) / d;
}

/**
 * @brief Converts a number of bits to the equivalent number of bytes, rounding up if necessary.
 *
 *
 * @param bits Number of bits.
 * @return The equivalent number of bytes, rounded up if necessary.
 */

extern size_t bits2bytes(size_t bits) {
    return divup(bits, bitsperbyte);
}

/**
 * @brief Converts a number of bytes to the equivalent number of bits.
 *
 *
 * @param e Number of bytes.
 * @return The equivalent number of bits.
 */

extern size_t e2size(int e) {
    return (size_t)1 << e;
}

/**
 * @brief Converts a size in bytes to the corresponding exponent e such that 2^e is the smallest power of two greater than or equal to the size.
 *
 *
 * @param size Size in bytes.
 * @return The corresponding exponent e.
 */

extern int size2e(size_t size) {
    int e = 0;
    while ((size_t)1 << e < size) {
        e++;
    }
    return e;
}

/**
 * @brief Sets a specific bit in a bitmap represented as an array of bytes.
 *
 *
 * @param p Pointer to the bitmap.
 * @param bit Bit number to be set.
 */

extern void bitset(void *p, int bit) {
    ((unsigned char *)p)[bit / bitsperbyte] |= (1 << (bit % bitsperbyte));
}

/**
 * @brief Clears a specific bit in a bitmap represented as an array of bytes.
 *
 *
 * @param p Pointer to the bitmap.
 * @param bit Bit number to be cleared.
 */

extern void bitclr(void *p, int bit) {
    ((unsigned char *)p)[bit / bitsperbyte] &= ~(1 << (bit % bitsperbyte));
}

/**
 * @brief Inverts a specific bit in a bitmap represented as an array of bytes.
 *
 *
 * @param p Pointer to the bitmap.
 * @param bit Bit number to be inverted.
 */

extern void bitinv(void *p, int bit) {
    ((unsigned char *)p)[bit / bitsperbyte] ^= (1 << (bit % bitsperbyte));
}

/**
 * @brief Tests a specific bit in a bitmap represented as an array of bytes.
 *
 *
 * @param p Pointer to the bitmap.
 * @param bit Bit number to be tested.
 */

extern int bittst(void *p, int bit) {
    return (((unsigned char *)p)[bit / bitsperbyte] >> (bit % bitsperbyte)) & 1;
}