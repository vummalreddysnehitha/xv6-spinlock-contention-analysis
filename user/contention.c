#include "kernel/types.h"
#include "user/user.h"

#define ITERATIONS 1000

int
main(void)
{
  uint64 before, after;
  int pid;

  before = getcontention();

  pid = fork();

  if (pid < 0) {
    printf("fork failed\n");
    exit(1);
  }

  if (pid == 0) {
    // Child: repeatedly request and release memory.
    for (int i = 0; i < ITERATIONS; i++) {
      char *p = sbrk(4096);

      if (p != (char *)-1) {
        p[0] = 'A';
        sbrk(-4096);
      }
    }

    exit(0);
  } else {
    // Parent: perform the same workload concurrently.
    for (int i = 0; i < ITERATIONS; i++) {
      char *p = sbrk(4096);

      if (p != (char *)-1) {
        p[0] = 'B';
        sbrk(-4096);
      }
    }

    wait(0);
  }

  after = getcontention();

  printf("Contention before workload: %lu\n", before);
  printf("Contention after workload:  %lu\n", after);
  printf("Contention during workload: %lu\n", after - before);

  exit(0);
}