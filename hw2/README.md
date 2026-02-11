# Memory Hole

Balloc is a memory allocator that implements the buddy system.  This system
allows for fast splitting and merging by dividing blocks into halves which are
called buddies.  It also incorperates the idea of a freelist to keep hold the
free blocks of memory.  It also uses a bitmap (bbm) to keep track of which
memory blocks are allocated versus free.

## Installation/Run

#### To run the deq tests:
1) Verify that there is a GNUMakefile in the parent directory of this project.  
2) Make the project
```bash
make deq
```
3) Run the executable
```bash
./deq
```

#### To run the balloc tests:
1) Make the project
```bash
gcc -o test_balloc test_balloc.c balloc.c bbm.c bm.c freelist.c utils.c -I.
```
2) Run the executable
```bash
./test_balloc
```

## Example output for test_balloc
```bash
Running buddy allocator tests...

Test: Basic allocation and freeing
  PASSED
Test: Multiple sequential allocations and freeing
  Allocated 16 bytes, got 16 bytes
  Allocated 32 bytes, got 32 bytes
  Allocated 64 bytes, got 64 bytes
  Allocated 128 bytes, got 128 bytes
  Allocated 256 bytes, got 256 bytes
  Allocated 512 bytes, got 512 bytes
  Allocated 100 bytes, got 128 bytes
  Allocated 200 bytes, got 256 bytes
  Allocated 300 bytes, got 512 bytes
  Allocated 400 bytes, got 512 bytes
  PASSED
Test: Allocation with data storage and retrieval
  Stored and retrieved: "Hello, buddy allocator!"
  PASSED
Test: Buddy coalescing on free
  Allocated two 128-byte blocks: sizes 128 and 128
  Freed both blocks (coalescing should occur)
  Successfully allocated larger block after coalescing
  PASSED
Test: Memory reuse after freeing
  First allocation: 0x7994022ff000
  Second allocation: 0x7994022ff000
  PASSED
Test: Size queries for different allocations
  Requested 10 bytes, allocated 16 bytes
  Requested 32 bytes, allocated 32 bytes
  Requested 100 bytes, allocated 128 bytes
  Requested 256 bytes, allocated 256 bytes
  Requested 1000 bytes, allocated 1024 bytes
  Requested 2048 bytes, allocated 2048 bytes
  PASSED
Test: Print statement for debugging
  Current state of buddy allocator:
Level 4: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Level 5: 00 00 00 00 00 00 00 00
Level 6: 00 00 00 01
Level 7: 00 01
Level 8: 01
Level 9: 01
Level 10: 01
Level 11: 01
Level 12: 01
Level 4: NULL
Level 5: NULL
Level 6: 0x7994022ff040 -> NULL
Level 7: NULL
Level 8: 0x7994022ff100 -> NULL
Level 9: 0x7994022ff200 -> NULL
Level 10: 0x7994022ff400 -> NULL
Level 11: 0x7994022ff800 -> NULL
Level 12: NULL
  State after freeing blocks:
Level 4: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00
Level 5: 00 00 00 00 00 00 00 00
Level 6: 00 00 00 00
Level 7: 00 00
Level 8: 00
Level 9: 00
Level 10: 00
Level 11: 00
Level 12: 01
Level 4: NULL
Level 5: NULL
Level 6: NULL
Level 7: NULL
Level 8: NULL
Level 9: NULL
Level 10: NULL
Level 11: NULL
Level 12: 0x7994022ff000 -> NULL
  PASSED

All tests passed!
```

## Example output for deq
```bash
Running Test: Push and Pop...
        Pass!
Running Test: Indexing (ith)...
        Pass!
Running Test: Specific Removal (rem)...
        Pass!
Running Test: String Output...
        Current Deq: [Keep1 Keep2]
Running Test: Index (i) out of bounds...
        Pass!
Running Test: Rem out of bounds...
        Pass!
Running Test: Getting an empty deq...
        Pass!
Running Test: rem until empty...
        Pass!
All tests passed successfully!
```