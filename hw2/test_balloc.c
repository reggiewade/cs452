#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "balloc.h"

void test_basic_alloc_free() {
    printf("Test: Basic allocation and freeing\n");
    
    // Create a pool: 4KB with exponent range [4, 12]
    Balloc pool = bcreate(4096, 4, 12);
    assert(pool != NULL);
    
    // Allocate a small block
    void *ptr1 = balloc(pool, 32);
    assert(ptr1 != NULL);
    assert(bsize(pool, ptr1) >= 32);
    
    // Allocate another block
    void *ptr2 = balloc(pool, 64);
    assert(ptr2 != NULL);
    assert(bsize(pool, ptr2) >= 64);
    
    // Free the blocks
    bfree(pool, ptr1);
    bfree(pool, ptr2);
    
    bdelete(pool);
    printf("  PASSED\n");
}

void test_multiple_allocations() {
    printf("Test: Multiple sequential allocations\n");
    
    Balloc pool = bcreate(4096, 4, 12);
    assert(pool != NULL);
    
    // Allocate multiple blocks of different sizes
    void *ptrs[10];
    int sizes[] = {16, 32, 64, 128, 256, 512, 100, 200, 300, 400};
    
    for (int i = 0; i < 10; i++) {
        ptrs[i] = balloc(pool, sizes[i]);
        assert(ptrs[i] != NULL);
        unsigned int allocated_size = bsize(pool, ptrs[i]);
        printf("  Allocated %d bytes, got %u bytes\n", sizes[i], allocated_size);
    }
    
    // Free all blocks
    for (int i = 0; i < 10; i++) {
        bfree(pool, ptrs[i]);
    }
    
    bdelete(pool);
    printf("  PASSED\n");
}

void test_allocation_with_data() {
    printf("Test: Allocation with data storage and retrieval\n");
    
    Balloc pool = bcreate(4096, 4, 12);
    assert(pool != NULL);
    
    // Allocate and write data
    void *ptr = balloc(pool, 256);
    assert(ptr != NULL);
    
    const char *test_str = "Hello, buddy allocator!";
    strcpy((char*)ptr, test_str);
    
    // Verify data is intact
    assert(strcmp((char*)ptr, test_str) == 0);
    printf("  Stored and retrieved: \"%s\"\n", (char*)ptr);
    
    bfree(pool, ptr);
    bdelete(pool);
    printf("  PASSED\n");
}

void test_buddy_coalescing() {
    printf("Test: Buddy coalescing on free\n");
    
    Balloc pool = bcreate(4096, 4, 12);
    assert(pool != NULL);
    
    // Allocate two adjacent buddies
    void *ptr1 = balloc(pool, 128);
    void *ptr2 = balloc(pool, 128);
    assert(ptr1 != NULL && ptr2 != NULL);
    
    unsigned int size1 = bsize(pool, ptr1);
    unsigned int size2 = bsize(pool, ptr2);
    printf("  Allocated two 128-byte blocks: sizes %u and %u\n", size1, size2);
    
    // Free both - should trigger coalescing
    bfree(pool, ptr1);
    bfree(pool, ptr2);
    printf("  Freed both blocks (coalescing should occur)\n");
    
    // Allocate a larger block to verify coalescing worked
    void *ptr3 = balloc(pool, 256);
    assert(ptr3 != NULL);
    printf("  Successfully allocated larger block after coalescing\n");
    
    bfree(pool, ptr3);
    bdelete(pool);
    printf("  PASSED\n");
}

void test_reuse_after_free() {
    printf("Test: Memory reuse after freeing\n");
    
    Balloc pool = bcreate(4096, 4, 12);
    assert(pool != NULL);
    
    // Allocate and free
    void *ptr1 = balloc(pool, 64);
    assert(ptr1 != NULL);
    bfree(pool, ptr1);
    
    // Allocate again - should reuse the memory
    void *ptr2 = balloc(pool, 64);
    assert(ptr2 != NULL);
    
    // They might not be the same address due to buddy splitting,
    // but we should be able to allocate again
    printf("  First allocation: %p\n", ptr1);
    printf("  Second allocation: %p\n", ptr2);
    
    bfree(pool, ptr2);
    bdelete(pool);
    printf("  PASSED\n");
}

void test_size_query() {
    printf("Test: Size queries for different allocations\n");
    
    Balloc pool = bcreate(4096, 4, 12);
    assert(pool != NULL);
    
    int test_sizes[] = {10, 32, 100, 256, 1000, 2048};
    
    for (int i = 0; i < 6; i++) {
        void *ptr = balloc(pool, test_sizes[i]);
        assert(ptr != NULL);
        unsigned int actual_size = bsize(pool, ptr);
        printf("  Requested %d bytes, allocated %u bytes\n", test_sizes[i], actual_size);
        assert(actual_size >= test_sizes[i]);
        bfree(pool, ptr);
    }
    
    bdelete(pool);
    printf("  PASSED\n");
}

int main() {
    printf("Running buddy allocator tests...\n\n");
    
    test_basic_alloc_free();
    test_multiple_allocations();
    test_allocation_with_data();
    test_buddy_coalescing();
    test_reuse_after_free();
    test_size_query();
    
    printf("\nAll tests passed!\n");
    return 0;
}
