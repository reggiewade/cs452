# Queue\<Anon>

Queue\<Anon> is a simple double ended queue that has functionality to add, remove and get elements
from either side of the queue.

## Installation

1) Verify that there is a GNUMakefile in the parent directory of this project.  
2) Make the project
```bash
make deq
```
3) Run the executable
```bash
./deq
```

## Valgrind
```bash
==3642962== 
==3642962== HEAP SUMMARY:
==3642962==     in use at exit: 0 bytes in 0 blocks
==3642962==   total heap usage: 32 allocs, 32 frees, 1,831 bytes allocated
==3642962== 
==3642962== All heap blocks were freed -- no leaks are possible
==3642962== 
==3642962== For lists of detected and suppressed errors, rerun with: -s
==3642962== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```