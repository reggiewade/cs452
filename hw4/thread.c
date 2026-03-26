#include <stdlib.h>
#include "thread.h"
#include "error.h"
pthread_t *thread_spawn(int n, void *(*f)(void *), void *arg) {
    pthread_t *threads = (pthread_t *)malloc(n * sizeof(pthread_t));
    if (!threads) ERROR("malloc() failed for thread array");

    for (int i = 0; i < n; i++) {
        if (pthread_create(&threads[i], NULL, f, arg) != 0) {
            ERROR("pthread_create() failed");
        }
    }
    return threads;
}

void thread_wait(int n, pthread_t *threads) {
    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            ERROR("pthread_join() failed");
        }
    }
    free(threads);
}