# Execution Integrity and Do Process

Execution Integrity and Do Process is a simple Unix shell with the ability to start
background tasks, piping and redirection.

## Installation

1) Verify that there is a GNUMakefile in the parent directory of this project.  
2) Run the tests
```bash
cd Test
./run
```
3) Make and run the executable
```bash
cd ~/hw3
make try
./try
```

## Test Ouptut:

*note: background tests will fail if 

```bash
Test_background
Test_background failed
Test_background_semi
Test_background_semi failed
Test_pipe
Test_pipe_semi
Test_pwd
Test_redirection
Test_semi
```

## Valgrind Output:

```bash
==71956== 
==71956== HEAP SUMMARY:
==71956==     in use at exit: 232,795 bytes in 234 blocks
==71956==   total heap usage: 547 allocs, 313 frees, 264,784 bytes allocated
==71956== 
==71956== LEAK SUMMARY:
==71956==    definitely lost: 0 bytes in 0 blocks
==71956==    indirectly lost: 0 bytes in 0 blocks
==71956==      possibly lost: 0 bytes in 0 blocks
==71956==    still reachable: 232,795 bytes in 234 blocks
==71956==         suppressed: 0 bytes in 0 blocks
==71956== Rerun with --leak-check=full to see details of leaked memory
==71956== 
==71956== For lists of detected and suppressed errors, rerun with: -s
==71956== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)```