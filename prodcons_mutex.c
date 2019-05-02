#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#define non_empty 0
#define non_full 1
#define BUF_MAX_SIZE 1000

/* 3 Threads: one is the producer, other are consumers */
pthread_t producer_thread;
pthread_t consumer_thread1, consumer_thread2;

/* State of the buffer is full or empty */
pthread_cond_t buffer_full;
pthread_cond_t buffer_empty;

/* Only a thread holding this mutex is allowed to access the buffer */
pthread_mutex_t mutex;

/* Who is allowed to access the buffer, the producer or the consumers? */
int state;

/* This buffer is shared between the threads */
char *buffer;

void producer() {
  char c[BUF_MAX_SIZE];

  printf("[P] Read up to %d chars (quit with 'end'): ", BUF_MAX_SIZE);
  for (;;) {

    if (0 == strcmp((fgets(c, BUF_MAX_SIZE, stdin)), "end\n"))
      pthread_exit(NULL);

    pthread_mutex_lock(&mutex);

    // !=non_full = full = non_empty
    while (state != non_full)
      pthread_cond_wait(&buffer_empty, &mutex);

    // Rewrite buffer
    memcpy(buffer, &c, BUF_MAX_SIZE);

    /* The buffer is full now, so tell the consumers */
    state = non_empty;

    pthread_cond_signal(&buffer_full);
    pthread_mutex_unlock(&mutex);
  }
}

void consumer(void *str) {
  char c[BUF_MAX_SIZE];

  for (;;) {
    pthread_mutex_lock(&mutex);

    while (state != non_empty)
      pthread_cond_wait(&buffer_full, &mutex);

    // Read data from buffer
    memcpy(&c, buffer, BUF_MAX_SIZE);

    // The buffer is empty now, so tell the producer
    state = non_full;

    pthread_cond_signal(&buffer_empty);
    pthread_mutex_unlock(&mutex);

    // display content, outside of critical section
    printf("[C]  %s = %s\n", (char *)str, c);
  }
}

int main() {
  char str1[] = "consumer1";
  char str2[] = "consumer2";

  /* The thread must start working */
  state = non_full;
  buffer = (char *)malloc(BUF_MAX_SIZE * sizeof(char));

  /* Initialize the 2 States (conditions) */
  pthread_cond_init(&buffer_full, NULL);
  pthread_cond_init(&buffer_empty, NULL);
  pthread_mutex_init(&mutex, NULL);

  /* And create the 3 threads */
  pthread_create(&producer_thread, NULL, (void *)&producer, NULL);
  pthread_create(&consumer_thread1, NULL, (void *)&consumer, (void *)str1);
  pthread_create(&consumer_thread2, NULL, (void *)&consumer, (void *)str2);

  pthread_detach(consumer_thread1);
  pthread_detach(consumer_thread2);
  pthread_join(producer_thread, NULL);

  free(buffer);

  return EXIT_SUCCESS;
}
