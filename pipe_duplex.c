#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define READ  0
#define WRITE 1

/* Duplex UNIX pipe example (slide 24):
 * Two pipes allow bidirectional communication between parent and child.
 * Parent sends an integer to child; child doubles it and sends result back.
 *
 * Steps:
 *   1. Create two pipes (pipe2child, pipe2parent)
 *   2. Fork child process
 *   3. Close unnecessary pipe ends
 *   4. Processing: parent writes, child reads/transforms/writes, parent reads result
 *   5. Close remaining pipes, parent waits for child, then exit
 */
int main(void) {
  int pipe2child[2];
  int pipe2parent[2];
  pid_t pid;

  /* Step 1: create pipes */
  if (pipe(pipe2child)) {
    perror("pipe call");
    exit(EXIT_FAILURE);
  }
  if (pipe(pipe2parent)) {
    perror("pipe call");
    exit(EXIT_FAILURE);
  }

  /* Step 2: fork */
  pid = fork();
  if (pid < (pid_t)0) { /* fork failed */
    perror("fork call");
    exit(EXIT_FAILURE);
  }

  if (pid == (pid_t)0) { /* code for child */
    /* Step 3: close unnecessary ends */
    close(pipe2child[WRITE]);
    close(pipe2parent[READ]);

    /* Step 4: read from parent, double it, write result back */
    int n;
    read(pipe2child[READ], &n, sizeof(int));
    fprintf(stdout, "[C] received %d, sending back %d\n", n, n * 2);
    int result = n * 2;
    write(pipe2parent[WRITE], &result, sizeof(int));

    /* Step 5: close remaining pipes and exit */
    close(pipe2child[READ]);
    close(pipe2parent[WRITE]);
    exit(EXIT_SUCCESS);
  }

  if (pid > (pid_t)0) { /* code for parent */
    /* Step 3: close unnecessary ends */
    close(pipe2child[READ]);
    close(pipe2parent[WRITE]);

    /* Step 4: send value to child, then read result */
    int n = 21;
    write(pipe2child[WRITE], &n, sizeof(int));
    fprintf(stdout, "[P] sent %d to child\n", n);
    int result;
    read(pipe2parent[READ], &result, sizeof(int));
    fprintf(stdout, "[P] received result %d from child\n", result);

    /* Step 5: close remaining pipes, wait for child, and exit */
    close(pipe2child[WRITE]);
    close(pipe2parent[READ]);
    waitpid(-1, NULL, 0);
    exit(EXIT_SUCCESS);
  }

  return EXIT_SUCCESS;
}
