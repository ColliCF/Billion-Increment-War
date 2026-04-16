# ============================================================
# Makefile - Billion-Increment-War
# Compila todos os experimentos (T1, T2, P1, P2)
# ============================================================

CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11
LIBS_T  = -lpthread
LIBS_P  = -lpthread

TARGETS = parte-b/p1_no_sync parte-b/p2_semaphore

# Adiciona t1/t2 se existirem na pasta parte-a ou na raiz
ifneq ($(wildcard t1.c),)
    TARGETS += t1
endif
ifneq ($(wildcard t2.c),)
    TARGETS += t2
endif

.PHONY: all clean run_p1_2 run_p1_4 run_p1_8 \
              run_p2_2 run_p2_4 run_p2_8 \
              benchmark_p

all: $(TARGETS)

parte-b/p1_no_sync: parte-b/p1_no_sync.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS_P)

parte-b/p2_semaphore: parte-b/p2_semaphore.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS_P)

# (Regras para t1 e t2 caso aplicavel)
t1: t1.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS_T)

t2: t2.c
	$(CC) $(CFLAGS) -o $@ $< $(LIBS_T)

# ------- Targets de execucao rapida -------
run_p1_2: parte-b/p1_no_sync
	@echo "=== P1 N=2 (sem sincronizacao) ==="
	time ./parte-b/p1_no_sync 2

run_p1_4: parte-b/p1_no_sync
	@echo "=== P1 N=4 (sem sincronizacao) ==="
	time ./parte-b/p1_no_sync 4

run_p1_8: parte-b/p1_no_sync
	@echo "=== P1 N=8 (sem sincronizacao) ==="
	time ./parte-b/p1_no_sync 8

run_p2_2: parte-b/p2_semaphore
	@echo "=== P2 N=2 (com semaforo POSIX) ==="
	time ./parte-b/p2_semaphore 2

run_p2_4: parte-b/p2_semaphore
	@echo "=== P2 N=4 (com semaforo POSIX) ==="
	time ./parte-b/p2_semaphore 4

run_p2_8: parte-b/p2_semaphore
	@echo "=== P2 N=8 (com semaforo POSIX) ==="
	time ./parte-b/p2_semaphore 8

benchmark_p: $(TARGETS)
	@echo ""
	@echo "=========================================="
	@echo " BENCHMARK - Parte B: Processos"
	@echo "=========================================="
	@for n in 2 4 8; do \
	    echo ""; \
	    echo "--- P1 N=$$n ---"; \
	    time ./parte-b/p1_no_sync $$n; \
	    echo ""; \
	    echo "--- P2 N=$$n ---"; \
	    time ./parte-b/p2_semaphore $$n; \
	done

clean:
	rm -f $(TARGETS)
