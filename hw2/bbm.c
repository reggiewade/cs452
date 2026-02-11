#include "bbm.h"
#include "bm.h"
#include "utils.h"


/**
 * @brief Calculates the number of bits needed in the bitmap to manage blocks of a given size and exponent level in a buddy allocator.
 *
 *
 * @param size Size of the memory pool.
 * @param e Exponent level of the blocks to be managed.
 * @return The number of bits needed in the bitmap.
 */

static size_t mapsize(size_t size, int e) {
  size_t blocksize=e2size(e);
  size_t blocks=divup(size,blocksize);
  size_t buddies=divup(blocks,2);
  return buddies;
}

/**
 * @brief Calculates the bit index in the bitmap corresponding to a given memory address and exponent level in a buddy allocator.
 *
 *
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 * @return The bit index in the bitmap corresponding to the given memory address and exponent level.
 */

static size_t bitaddr(void *base, void *mem, int e) {
  size_t addr=baddrclr(base,mem,e)-base;
  size_t blocksize=e2size(e);
  return addr/blocksize/2;
}

/**
 * @brief Creates a new bitmap for managing blocks of memory.
 *
 *
 * @param size Size of the memory pool.
 * @param e Exponent level of the blocks to be managed.
 * @return A pointer to the newly created bitmap, or NULL if allocation fails.
 */
extern BBM bbmcreate(size_t size, int e) {
  return bmcreate(mapsize(size,e));
}

/**
 * @brief Deletes a bitmap, freeing all associated memory back to the system.
 *
 *
 * @param b Pointer to the bitmap.
 */
extern void bbmdelete(BBM b) {
  bmdelete(b);
}

/**
 * @brief Sets a specific bit in the bitmap corresponding to memory address.
 *
 *
 * @param b Pointer to the bitmap.
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 */

extern void bbmset(BBM b, void *base, void *mem, int e) {
  bmset(b,bitaddr(base,mem,e));
}

/**
 * @brief Clears a specific bit in the bitmap corresponding to memory address.
 *
 *
 * @param b Pointer to the bitmap.
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 */

extern void bbmclr(BBM b, void *base, void *mem, int e) {
  bmclr(b,bitaddr(base,mem,e));
}

/**
 * @brief Tests a specific bit in the bitmap corresponding to memory address.
 *
 *
 * @param b Pointer to the bitmap.
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 */

extern int bbmtst(BBM b, void *base, void *mem, int e) {
  return bmtst(b,bitaddr(base,mem,e));
}

/**
 * @brief Prints the contents of the bitmap in human-readable format for debugging purposes.
 *
 *
 * @param b Pointer to the bitmap.
 */

extern void bbmprt(BBM b) { bmprt(b); }

/**
 * @brief Calculates the right buddy address for a given memory block and exponent level in a buddy allocator.
 *
 *
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 */

extern void *baddrset(void *base, void *mem, int e) {
  unsigned int mask=1<<e;
  return base+((mem-base)|mask);
}

/**
 * @brief Calculates the left buddy's memory address by clearing the bit corresponding to the exponent level.
 *
 *
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 */

extern void *baddrclr(void *base, void *mem, int e) {
  unsigned int mask=~(1<<e);
  return base+((mem-base)&mask);
}

/**
 * @brief Calculates the buddy's memory address by inverting the bit corresponding to the exponent level.
 *
 *
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 */

extern void *baddrinv(void *base, void *mem, int e) {
  unsigned int mask=1<<e;
  return base+((mem-base)^mask);
}

/**
 * @brief Calculates the buddy's memory address by testing the bit corresponding to the exponent level and returning the appropriate buddy address.
 *
 *
 * @param base Base address of the memory pool.
 * @param mem Pointer to the memory block being managed.
 * @param e Exponent level of the block being managed.
 */

extern int baddrtst(void *base, void *mem, int e) {
  unsigned int mask=1<<e;
  return (mem-base)&mask;
}
