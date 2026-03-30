#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

#include "wrapper.h"
#include "lawn.h"
#include "mole.h"
#include "thread.h" 

/**
 * @brief Creates and queues a new mole
 * 
 * @param a An array of arguments, where a[0] is the Mtq queue and a[1] is the Lawn.
 *
 */
static void *produce(void *a) {
  void **arg = a;
  Mtq queue = (Mtq)arg[0];
  Lawn lawn = (Lawn)arg[1];
  mtq_tail_put(queue, mole_new(lawn, 0, 0));
  return 0;
}

/**
 * @brief Consumes a mole from the queue and whacks it
 * 
 * @param a An array of arguments, where a[0] is the Mtq queue and a[1] is the Lawn.
 *
 */
static void *consume(void *a) {
  void **arg = a;
  Mtq queue = (Mtq)arg[0];
  
  Mole m = mtq_head_get(queue);
  if (m) {
    mole_whack(m);
  }
  return 0;
}

int main() {
  srandom(time(0));
  const int n = 10;
  const int capacity = 4;
  
  Lawn lawn = lawn_new(0, 0);
  
  Mtq q = mtq_new(capacity); 

  void *args[2] = { (void *)q, (void *)lawn };

  pthread_t *prods = thread_spawn(n, produce, args);
  pthread_t *cons = thread_spawn(n, consume, args);

  thread_wait(n, prods);
  thread_wait(n, cons);

  mtq_del(q); 
  lawn_free(lawn);
  
  return 0;
}