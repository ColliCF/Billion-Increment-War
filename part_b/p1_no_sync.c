#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define BILLION 1000000000LL

typedef struct {
    volatile long long counter;
} SharedData;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Uso: %s <N>\n", argv[0]);
        return EXIT_FAILURE;
    }
    int N = atoi(argv[1]);
    if (N <= 0) {
        fprintf(stderr, "Erro: N deve ser positivo.\n");
        return EXIT_FAILURE;
    }
    long long wpw = BILLION / N;

    int shmid = shmget(IPC_PRIVATE, sizeof(SharedData), IPC_CREAT | 0666);
    if (shmid == -1) { perror("shmget"); return EXIT_FAILURE; }

    SharedData *shm = (SharedData *)shmat(shmid, NULL, 0);
    if (shm == (SharedData *)-1) {
        perror("shmat");
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }
    shm->counter = 0;

    printf("[P1] %d processo(s) SEM sincronizacao | meta=%lld | wpw=%lld\n",
           N, BILLION, wpw);

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            shmctl(shmid, IPC_RMID, NULL);
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            for (long long j = 0; j < wpw; j++)
                shm->counter++;
            shmdt(shm);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < N; i++) wait(NULL);

    printf("[P1] counter   = %lld\n", shm->counter);
    printf("[P1] esperado  = %lld\n", BILLION);
    printf("[P1] diferenca = %lld  (corrupcao por race condition)\n",
           BILLION - shm->counter);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
