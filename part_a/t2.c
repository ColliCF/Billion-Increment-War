#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TARGET 1000000000

long long counter = 0;
pthread_mutex_t lock;
long long work_per_thread;

void* increment_task(void* arg) {
    for (long long i = 0; i < work_per_thread; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    int n_threads = (argc > 1) ? atoi(argv[1]) : 2;
    work_per_thread = TARGET / n_threads;

    pthread_t threads[n_threads];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < n_threads; i++) {
        pthread_create(&threads[i], NULL, increment_task, NULL);
    }

    for (int i = 0; i < n_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter: %lld\n", counter);
    printf("Expected: %d\n", TARGET);

    pthread_mutex_destroy(&lock);
    return 0;
}