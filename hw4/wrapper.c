#include <pthread.h>
#include <stdlib.h>
#include "wrapper.h"

struct Mtq {
  Deq q;
  int capacity;
  pthread_mutex_t lock;
  pthread_cond_t  can_produce;
  pthread_cond_t  can_consume;
};

/**
 * @brief Creates a new Multi-threaded Queue (MTQ) with the given capacity.
 * 
 * @param capacity The maximum number of elements the queue can hold.
 * @return A pointer to the newly created Mtq instance.
 *
 */
Mtq mtq_new(int capacity) {
  Mtq q = malloc(sizeof(*q));
  q->q = deq_new();
  q->capacity = capacity;
  pthread_mutex_init(&q->lock, NULL);
  pthread_cond_init(&q->can_produce, NULL);
  pthread_cond_init(&q->can_consume, NULL);
  return q;
}

/**
 * @brief Adds an element to the specified end of the deque.
 *
 * @param q The multi-threaded queue to which the element is to be added.
 * @param d The data to be added to the queue.
 */
void mtq_tail_put(Mtq q, Data d) {
  pthread_mutex_lock(&q->lock);

  while (q->capacity > 0 && deq_len(q->q) >= q->capacity) {
    pthread_cond_wait(&q->can_produce, &q->lock);
  }

  deq_tail_put(q->q, d);

  pthread_cond_signal(&q->can_consume);
  pthread_mutex_unlock(&q->lock);
}

/**
 * @brief Retrieves and removes an element from the head of the deque.
 *
 * @param q The multi-threaded queue from which the element is to be retrieved.
 * @return The data removed from the queue.
 */
Data mtq_head_get(Mtq q) {
  pthread_mutex_lock(&q->lock);

  while (deq_len(q->q) == 0) {
    pthread_cond_wait(&q->can_consume, &q->lock);
  }

  Data d = deq_head_get(q->q);

  pthread_cond_signal(&q->can_produce);
  pthread_mutex_unlock(&q->lock);
  return d;
}

/**
 * @brief Deletes the multi-threaded queue and frees all associated resources.
 *
 * @param q The multi-threaded queue to be deleted.
 */
void mtq_del(Mtq q) {
  if (!q) return;

  deq_del(q->q, 0); 

  pthread_mutex_destroy(&q->lock);
  pthread_cond_destroy(&q->can_produce);
  pthread_cond_destroy(&q->can_consume);

  free(q);
}