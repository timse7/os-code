#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char **ptr; /* global */

/* thread routine */
void *thread(void *vargp) {
  int myid = (int)vargp;
  static int cnt = 0;

  printf("[%d]: %s (cnt=%d)\n", myid, ptr[myid], ++cnt);

  pthread_exit(NULL);
}

int main() { /* main thread */
  int i;
  pthread_t tid[2];
  char *msgs[100] = {"Hello from foo", "Hello from bar"};
  ptr = msgs;
  for (i = 0; i < 2; i++)
    pthread_create(&tid[i], NULL, thread, (void *)i);
  pthread_exit(NULL);
}
