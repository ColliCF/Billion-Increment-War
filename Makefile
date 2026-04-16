CC      = gcc
CFLAGS  = -O2 -Wall -Wextra -std=c11
LIBS_T  = -lpthread
LIBS_P  = -lpthread

TARGETS = compiled/p1 compiled/p2 \
          compiled/t1 compiled/t2

.PHONY: all clean run-partA run-partB benchmark

all: $(TARGETS)

compiled/p1: part_b/p1.c
	mkdir -p compiled
	$(CC) $(CFLAGS) -o $@ $< $(LIBS_P)

compiled/p2: part_b/p2.c
	mkdir -p compiled
	$(CC) $(CFLAGS) -o $@ $< $(LIBS_P)

compiled/t1: part_a/t1.c
	mkdir -p compiled
	$(CC) -o $@ $< $(LIBS_T)

compiled/t2: part_a/t2.c
	mkdir -p compiled
	$(CC) -o $@ $< $(LIBS_T)

run-partA: compiled/t1 compiled/t2
	./run_partA.sh

run-partB: compiled/p1 compiled/p2
	./run_partB.sh

benchmark: run-partA run-partB
	@echo ""
	@echo "Resultados em results.csv"

clean:
	rm -f $(TARGETS)
	rm -f results.csv
