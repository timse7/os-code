#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MYPIPE "/tmp/os_code_fifo"

/* Named pipe (FIFO) example (slides 25-26):
 * Same parent-to-child transfer as simple_pipe.c, but using a FIFO.
 *
 * Key differences to simple_pipe.c:
 *   - mkfifo() creates a named pipe in the filesystem instead of pipe()
 *   - fopen() with the right permission flag replaces closing unused ends
 *   - The FIFO persists after use and must be unlinked explicitly
 *   - Processes of different ancestry could share the same FIFO
 */
int main(void) {
  pid_t pid;

  /* Create the FIFO (fails with EEXIST if already present, so unlink first) */
  unlink(MYPIPE);
  if (mkfifo(MYPIPE, 0666) < 0) {
    perror("mkfifo");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid < (pid_t)0) { /* fork failed */
    perror("fork call");
    exit(EXIT_FAILURE);
  }

  if (pid == (pid_t)0) { /* code for child */
    /* Open FIFO for reading — blocks until parent opens for writing */
    FILE *fp = fopen(MYPIPE, "r");
    if (!fp) {
      perror("fopen");
      exit(EXIT_FAILURE);
    }
    int myPipe = fileno(fp);
    int n;
    read(myPipe, &n, sizeof(int));
    fprintf(stdout, "[C] read %d from parent\n", n);
    fclose(fp);
    exit(EXIT_SUCCESS);
  }

  /* code for parent */
  /* Open FIFO for writing — blocks until child opens for reading */
  FILE *fp = fopen(MYPIPE, "w");
  if (!fp) {
    perror("fopen");
    exit(EXIT_FAILURE);
  }
  int myPipe = fileno(fp);
  int n = 42;
  write(myPipe, &n, sizeof(int));
  fprintf(stdout, "[P] write %d to child\n", n);
  fclose(fp);
  waitpid(-1, NULL, 0);

  /* Remove the FIFO from the filesystem */
  unlink(MYPIPE);
  exit(EXIT_SUCCESS);
}
