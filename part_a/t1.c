#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define TARGET 1000000000
long long cont = 0;
long long work_per_thread;

void* increment(){
    for (long long i = 0; i < work_per_thread; i++){
        cont++;
    }
    return NULL;
}

int main(int argc, char* argv[]){
    int n_threads = (argc > 1) ? atoi(argv[1]) : 2;
    work_per_thread = TARGET / n_threads;

    pthread_t threads[n_threads];

    for (int i = 0; i < n_threads; i++){
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    for(int i = 0; i < n_threads; i++){
        pthread_join(threads[i], NULL);
    }

    printf("Final counter: %lld\n", cont);
    printf("Expected: %d\n", TARGET);

    return 0;
}