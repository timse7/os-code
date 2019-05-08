#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#define MSGSIZE 80

int msqid;         /* Message queue identifier */
struct my_msgbuf { /* Our messages have that look */
  long mtype;
  char message[MSGSIZE]; /* The real message-text */
};

/* Remove the message queue from the kernel */
void killmsgbuf() { msgctl(msqid, IPC_RMID, 0); }

/* Read junk from stdin and place it in the message queue */
void producer() {

  struct my_msgbuf pmbuf;

  pmbuf.mtype = 1;
  for (;;) {
    printf(": ");
    gets(pmbuf.message);
    if (0 == strcmp("end", pmbuf.message)) {
      /* Tell consumer that we wish to end the show now */
      msgsnd(msqid, &pmbuf, MSGSIZE, 0);
      break;
    }
    /* Messages may be lost if the message queue is full ! */
    if (-1 == (msgsnd(msqid, &pmbuf, MSGSIZE, IPC_NOWAIT)))
      perror("\nMessage NOT delivered!\n");
  }
}

/* Read messages from the queue and write them into a file */
void consumer() {

  struct my_msgbuf cmbuf;
  FILE *fd;

  fd = fopen("consumer.out", "w");
  for (;;) {
    msgrcv(msqid, &cmbuf, MSGSIZE, 0, 0);
    if (0 == (strcmp("end", cmbuf.message))) {
      /* consumer has to kill the message buffer because he
         is the last who will access it */
      killmsgbuf();
      fclose(fd);
      break;
    }
    fprintf(fd, "%s\n", cmbuf.message);
  }
}

/* Start the ball rolling */
int main() {

  if (-1 == (msqid = msgget((ftok(".", '1')), IPC_CREAT | 0666))) {
    perror("msgget");
    exit(-1);
  }

  if (0 == (fork())) {
    consumer();
  } else {
    producer();
  }

  exit(0);
}
