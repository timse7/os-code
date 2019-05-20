#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int sum;

void *runner(void *param);

int main(int argc, char *argv[]) {
  pthread_t tid;
  pthread_attr_t attr;

  if (argc != 2) {
    fprintf(stderr, "usage: %s <count>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  pthread_attr_init(&attr);
  pthread_create(&tid, &attr, runner, argv[1]);
  pthread_join(tid, NULL);

  fprintf(stdout, "sum = %d\n", sum);
  exit(EXIT_SUCCESS);
}

void *runner(void *param) {
  int i, upper = strtol(param, NULL, 10);
  sum = 0;
  for (i = 1; i <= upper; i++)
    sum += i;
  pthread_exit(EXIT_SUCCESS);
}
