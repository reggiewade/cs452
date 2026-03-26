#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "wrapper.h"      // Use the MT-safe wrapper header
#include "lawn.h"
#include "mole.h"
#include "thread.h" 

// Updated produce to use Mtq
static void *produce(void *a) {
  void **arg = a;
  Mtq q = (Mtq)arg[0]; // Cast to Mtq type
  Lawn l = (Lawn)arg[1];
  mtq_tail_put(q, mole_new(l, 0, 0)); // Call wrapper function
  return 0;
}

// Updated consume to use Mtq
static void *consume(void *a) {
  void **arg = a;
  Mtq q = (Mtq)arg[0]; // Cast to Mtq type
  
  Mole m = mtq_head_get(q); // Call wrapper function
  if (m) {
    mole_whack(m);
  }
  return 0;
}

int main() {
  srandom(time(0));
  const int n = 10;
  const int capacity = 4; // As suggested by the assignment
  
  Lawn lawn = lawn_new(0, 0);
  
  // Initialize the MT-safe queue with a capacity
  Mtq q = mtq_new(capacity); 

  void *args[2] = { (void *)q, (void *)lawn };

  // Launch threads using your thread module
  pthread_t *prods = thread_spawn(n, produce, args);
  pthread_t *cons = thread_spawn(n, consume, args);

  // Wait for completion
  thread_wait(n, prods);
  thread_wait(n, cons);

  // Cleanup using the wrapper's delete function
  mtq_del(q); 
  lawn_free(lawn);
  
  return 0;
}