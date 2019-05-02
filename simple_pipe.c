#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define READ 0
#define WRITE 1

int main(int argc, char **argv) {

  int myPipe[2];
  pid_t pid;

  if (pipe(myPipe)) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid < (pid_t)0) { /* fork failed */
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == (pid_t)0) { /* code for child */
    close(myPipe[WRITE]);
    int n;
    read(myPipe[READ], &n, sizeof(int));
    fprintf(stdout, "[C] read %d from parent\n", n);
    close(myPipe[READ]);
    exit(EXIT_SUCCESS);
  }

  /* code for parent */
  close(myPipe[READ]);
  int n = 42;
  write(myPipe[WRITE], &n, sizeof(int));
  fprintf(stdout, "[P] write %d to child\n", n);
  close(myPipe[WRITE]);
  waitpid(-1, NULL, 0);
  exit(EXIT_SUCCESS);
};
