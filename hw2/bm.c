#include <stdlib.h>
#include <string.h>

#include "bm.h"
#include "utils.h"

static size_t bmbits(BM b) { size_t *bits=b; return *--bits; }

static size_t bmbytes(BM b) { return bits2bytes(bmbits(b)); }

/**
 * @brief Checks if the given index is within the bounds of the bitmap and exits with an error message if it is out of range.
 *
 *
 * @param b Pointer to the bitmap.
 * @param i Bit index to be checked.
 */

static void ok(BM b, size_t i) {
  if (i<bmbits(b))
    return;
  fprintf(stderr,"bitmap index out of range\n");
  exit(1);
} 

/**
 * @brief Creates a new bitmap with the specified number of bits, allocating the necessary memory and initializing all bits to 0.
 *
 *
 * @param bits Number of bits in the new bitmap.
 * @return Pointer to the newly created bitmap, or 0 if allocation fails.
 */

extern BM bmcreate(size_t bits) {
  size_t bytes=bits2bytes(bits);
  size_t *p=mmalloc(sizeof(size_t)+bytes);
  if ((long)p==-1)
    return 0;
  *p=bits;
  BM b=++p;
  memset(b,0,bytes);
  return b;
}

/**
 * @brief Deletes a bitmap, freeing all associated memory back to the system.
 *
 *
 * @param b Pointer to the bitmap to be deleted.
 */

extern void bmdelete(BM b) {
  size_t *p=b;
  p--;
  mmfree(p,*p);
}

/**
 * @brief Sets a specific bit in a bitmap represented as an array of bytes.
 *
 *
 * @param b Pointer to the bitmap.
 * @param i Bit number to be set.
 */

extern void bmset(BM b, size_t i) {
  ok(b,i); bitset(b+i/bitsperbyte,i%bitsperbyte);
}

/**
 * @brief Clears a specific bit in a bitmap represented as an array of bytes.
 *
 *
 * @param b Pointer to the bitmap.
 * @param i Bit number to be cleared.
 */

extern void bmclr(BM b, size_t i) {
  ok(b,i); bitclr(b+i/bitsperbyte,i%bitsperbyte);
}

/**
 * @brief Tests a specific bit in the bitmap, returning 1 if the bit is set and 0 if it is clear.
 *
 *
 * @param b pointer to the bitmap.
 * @param i Bit number to be tested.
 * @return 1 if the bit is set, 0 if it is clear.
 */

extern int bmtst(BM b, size_t i) {
  ok(b,i); return bittst(b+i/bitsperbyte,i%bitsperbyte);
}

/**
 * @brief Prints the contents of the bitmap in human-readable format.
 *
 *
 * @param b Pointer to the bitmap.
 */

extern void bmprt(BM b) {
  for (int byte=bmbytes(b)-1; byte>=0; byte--)
    printf("%02x%s",((char *)b)[byte],(byte ? " " : "\n"));
}
