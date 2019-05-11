#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#define MSGSIZE 100
#define FULLBASH "/bin/bash"
#define BASH "bash"
#define BASHOPTION "-c"

int main(int argc, char **argv) {

  pid_t pid;
  char cmd[100];

  printf("Welcome to ctshell (end with 'quit'). Have fun..\n");

  while (1) {
    printf("ctsh$ ");

    fgets(cmd, MSGSIZE, stdin);
    if (cmd[strlen(cmd) - 1] == '\n')
      cmd[strlen(cmd) - 1] = '\0';
    if (0 == strcmp("quit", cmd)) {
      printf("Bye, bye...\n");
      exit(EXIT_SUCCESS);
    }

    pid = fork();
    if (pid < (pid_t)0) { /* fork failed */
      perror("fork");
      exit(EXIT_FAILURE);
    }

    if (pid == (pid_t)0) { /* code for child */
      execl(FULLBASH, BASH, BASHOPTION, cmd, NULL);
      fprintf(stderr, "ctsh$ ***error*** cannot execute %s\n", cmd);
      exit(EXIT_FAILURE);
    }

    if (pid > (pid_t)0) { /* code for parent */
      waitpid(-1, NULL, 0);
    }
  }
};
