#include <stdlib.h>
#include "thread.h"
#include "error.h"

/**
 * @brief Spawns multiple threads to execute a given function with the provided argument.
 *
 * This function creates multiple threads that will execute the specified function
 * with the provided argument.
 *
 * @param n The number of threads to spawn.
 * @param f The function to execute in each thread.
 * @param arg The argument to pass to each thread.
 * @return A pointer to an array of thread identifiers.
 */
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

/**
 * @brief Waits for multiple threads to complete their execution.
 *
 * This function waits for all the specified threads to finish their execution.
 *
 * @param n The number of threads to wait for.
 * @param threads An array of thread identifiers.
 */
void thread_wait(int n, pthread_t *threads) {
    for (int i = 0; i < n; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            ERROR("pthread_join() failed...");
        }
    }
    free(threads);
}