#include <pthread.h>
#include <stdio.h>
#define n 1000000000

int cont = 0;

void* increment(void* arg){
    while (cont < n){
        cont++;
    }
    return NULL;
}

int main(void){
    int n_threads = 5;
    pthread_t threads[n_threads];

    for (int i = 0; i<n_threads; i++){
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    for(int i = 0; i<n_threads; i++){
        pthread_join(threads[i], NULL);
    }

    printf("%d\n", cont);
}