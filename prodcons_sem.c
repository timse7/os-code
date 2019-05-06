#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <semaphore.h>

#define BUFFERSIZE 10

/* 3 threads, one producer,
 * 2 consumers */
pthread_t producer_thread;
pthread_t consumer_thread1, consumer_thread2;

/* A protected buffer,
 * protected with 3 semaphores */
typedef struct prot_buffer {
  sem_t *sem_read;
  sem_t *sem_write;
  int readpos, writepos;
  char buffer[BUFFERSIZE];
} prot_buffer_t;

prot_buffer_t *b;

sem_t *mutex;

void *producer(void *str) {
  printf("[P] read characters and put into shared buffer (quit with 'E')...\n");

#ifdef DEBUG
  printf("[P] %s started...\n", (char *)str);
#endif
  char c;
  for (;;) {
    while ('\n' != (c = getchar())) {
      if ('E' == c) {
        pthread_cancel(consumer_thread1);
        pthread_cancel(consumer_thread2);
        pthread_exit(NULL);
      }
#ifdef DEBUG
      printf("[P] %s put %c\n", (char *)str, c);
#endif
      if (sem_wait(b->sem_write))
        perror("sem_wait");
      if (sem_wait(mutex))
        perror("sem_wait");

#ifdef DEBUG
      printf("[P] %s in critical section...\n", (char *)str);
#endif
      b->buffer[b->writepos] = c;
      b->writepos++;
      if (b->writepos >= BUFFERSIZE)
        b->writepos = 0;
#ifdef DEBUG
      printf("[P] %s about to leave  critical section...\n", (char *)str);
#endif

      if (sem_post(mutex))
        perror("sem_post");
      if (sem_post(b->sem_read))
        perror("sem_post");
#ifdef DEBUG
      printf("[P] %s outside critical section...\n", (char *)str);
#endif
    }
  }
}

void *consumer(void *str) {
#ifdef DEBUG
  printf("[C] %s started...\n", (char *)str);
#endif
  char c;
  for (;;) {
#ifdef DEBUG
    printf("[C] %s trying to enter critical section ...\n", (char *)str);
#endif
    if (sem_wait(b->sem_read))
      perror("sem_wait");
    if (sem_wait(mutex))
      perror("sem_wait");

#ifdef DEBUG
    printf("[C] %s in critical section...\n", (char *)str);
#endif
    c = b->buffer[b->readpos];
    b->readpos++;
    if (b->readpos >= BUFFERSIZE)
      b->readpos = 0;

    if (sem_post(mutex))
      perror("sem_post");
    if (sem_post(b->sem_write))
      perror("sem_post");

#ifdef DEBUG
    printf("[C] %s outside critical section...\n", (char *)str);
#endif
    printf(" %s: ", (char *)str);
    putchar(c);
    putchar('\n');
  }
}

int main(int argc, char *argv[]) {
  char str0[] = "producer";
  char str1[] = "consumer1";
  char str2[] = "consumer2";

#ifdef DEBUG
  printf("[M] start...\n");
#endif
  b = (prot_buffer_t *)malloc(sizeof(prot_buffer_t));
  b->readpos = 0;
  b->writepos = 0;
  b->sem_read =
      sem_open("/AAU_T3_READ_SS2019", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 0);
  b->sem_write = sem_open("/AAU_T3_WRITE_SS2019", O_CREAT | O_EXCL,
                          S_IRUSR | S_IWUSR, BUFFERSIZE - 1);
  mutex =
      sem_open("/AAU_T3_MUTEX_SS2019", O_CREAT | O_EXCL, S_IRUSR | S_IWUSR, 1);
  if (b->sem_read == SEM_FAILED || b->sem_write == SEM_FAILED ||
      mutex == SEM_FAILED) {
    perror("sem_open");
    return EXIT_FAILURE;
  }

#ifdef DEBUG
  printf("[M] prot_buffer created...\n");
#endif
  pthread_create(&producer_thread, NULL, producer, str0);
  pthread_create(&consumer_thread1, NULL, consumer, str1);
  pthread_create(&consumer_thread2, NULL, consumer, str2);

#ifdef DEBUG
  printf("[M] threads started and now waiting for threads to join...\n");
#endif
  // join waits for the threads to finish
  pthread_detach(consumer_thread1);
  pthread_detach(consumer_thread2);
  pthread_join(producer_thread, NULL);

#ifdef DEBUG
  printf("[M] close all semaphores...\n");
#endif
  if (sem_close(b->sem_read))
    perror("sem_close");
  if (sem_close(b->sem_write))
    perror("sem_close");
  if (sem_close(mutex))
    perror("sem_close");
#ifdef DEBUG
  printf("[M] unlink all semaphores...\n");
#endif
  if (sem_unlink("/AAU_T3_READ_SS2019"))
    perror("sem_unlink");
  if (sem_unlink("/AAU_T3_WRITE_SS2019"))
    perror("sem_unlink");
  if (sem_unlink("/AAU_T3_MUTEX_SS2019"))
    perror("sem_unlink");
  free(b);
#ifdef DEBUG
  printf("[M] ready to termine...\n");
#endif
  return EXIT_SUCCESS;
}
