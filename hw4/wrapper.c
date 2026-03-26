#include <pthread.h>
#include <stdlib.h>
#include "wrapper.h"

struct Mtq {
  Deq q;
  int capacity; // 0 means unbounded
  pthread_mutex_t lock;
  pthread_cond_t  can_produce; // Signal when items are removed
  pthread_cond_t  can_consume; // Signal when items are added
};

Mtq mtq_new(int capacity) {
  Mtq q = malloc(sizeof(*q));
  q->q = deq_new();
  q->capacity = capacity;
  pthread_mutex_init(&q->lock, NULL);
  pthread_cond_init(&q->can_produce, NULL);
  pthread_cond_init(&q->can_consume, NULL);
  return q;
}

void mtq_tail_put(Mtq q, Data d) {
  pthread_mutex_lock(&q->lock);

  // If capacity > 0 and queue is full, WAIT
  while (q->capacity > 0 && deq_len(q->q) >= q->capacity) {
    pthread_cond_wait(&q->can_produce, &q->lock);
  }

  deq_tail_put(q->q, d);

  // Signal any waiting consumers that there is now data
  pthread_cond_signal(&q->can_consume);
  pthread_mutex_unlock(&q->lock);
}

Data mtq_head_get(Mtq q) {
  pthread_mutex_lock(&q->lock);

  // Wait until there is actually something to get
  while (deq_len(q->q) == 0) {
    pthread_cond_wait(&q->can_consume, &q->lock);
  }

  Data d = deq_head_get(q->q);

  // Signal any waiting producers that there is now space
  pthread_cond_signal(&q->can_produce);
  pthread_mutex_unlock(&q->lock);
  return d;
}

void mtq_del(Mtq q) {
  if (!q) return;

  // 1. Destroy the underlying MT-unsafe queue
  // We pass 0 (NULL) because the moles are handled by mole_whack
  deq_del(q->q, 0); 

  // 2. Clean up the synchronization objects
  pthread_mutex_destroy(&q->lock);
  pthread_cond_destroy(&q->can_produce);
  pthread_cond_destroy(&q->can_consume);

  // 3. Free the wrapper structure itself
  free(q);
}