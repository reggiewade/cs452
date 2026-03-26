#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>

pthread_t *thread_spawn(int n, void *(*f)(void *), void *arg);

void thread_wait(int n, pthread_t *threads);

#endif