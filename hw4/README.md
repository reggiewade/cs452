# Whack-a-Mole and Race Conditions

This program simulates the classic whack-a-mole game and continuously creates moles at random positions
on a lawn while simultaneously the moles are removed and "whacked"

## Installation

1) Verify that there is a GNUMakefile in the parent directory of this project.  

2) Make and run the executable
```bash
cd ~/hw4
make wam
./wam
```


## Valgrind Output:

```bash
==9601== Memcheck, a memory error detector
==9601== Copyright (C) 2002-2022, and GNU GPL'd, by Julian Seward et al.
==9601== Using Valgrind-3.22.0 and LibVEX; rerun with -h for copyright info
==9601== Command: ./wam
==9601== 
==9601== 
==9601== HEAP SUMMARY:
==9601==     in use at exit: 139,113 bytes in 1,132 blocks
==9601==   total heap usage: 2,267 allocs, 1,135 frees, 313,108 bytes allocated
==9601== 
==9601== LEAK SUMMARY:
==9601==    definitely lost: 0 bytes in 0 blocks
==9601==    indirectly lost: 0 bytes in 0 blocks
==9601==      possibly lost: 0 bytes in 0 blocks
==9601==    still reachable: 139,113 bytes in 1,132 blocks
==9601==         suppressed: 0 bytes in 0 blocks
==9601== Rerun with --leak-check=full to see details of leaked memory
==9601== 
==9601== For lists of detected and suppressed errors, rerun with: -s
==9601== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)```