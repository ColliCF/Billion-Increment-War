#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>

#define BILLION   1000000000LL
#define SEM_NAME  "/billion_war_sem"

typedef struct {
    long long counter;
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

    sem_unlink(SEM_NAME);

    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        shmdt(shm);
        shmctl(shmid, IPC_RMID, NULL);
        return EXIT_FAILURE;
    }

    printf("[P2] %d processo(s) COM semaforo POSIX | meta=%lld | wpw=%lld\n",
           N, BILLION, wpw);

    for (int i = 0; i < N; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            sem_close(sem); sem_unlink(SEM_NAME);
            shmdt(shm); shmctl(shmid, IPC_RMID, NULL);
            return EXIT_FAILURE;
        }
        if (pid == 0) {
            sem_t *cs = sem_open(SEM_NAME, 0);
            if (cs == SEM_FAILED) {
                perror("sem_open child");
                shmdt(shm);
                exit(EXIT_FAILURE);
            }

            for (long long j = 0; j < wpw; j++) {
                sem_wait(cs);
                shm->counter++;
                sem_post(cs);
            }

            sem_close(cs);
            shmdt(shm);
            exit(EXIT_SUCCESS);
        }
    }

    for (int i = 0; i < N; i++) wait(NULL);

    printf("[P2] counter   = %lld\n", shm->counter);
    printf("[P2] esperado  = %lld\n", BILLION);
    printf("[P2] diferenca = %lld\n", BILLION - shm->counter);

    sem_close(sem);
    sem_unlink(SEM_NAME);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return EXIT_SUCCESS;
}
