#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static void sig_handler(int signo) {
  switch (signo) {
    case SIGUSR1:  /* Incoming SIGUSR1 signal */
      printf("Parent: Received %d\n", SIGUSR1);
      break;
    case SIGUSR2:  /* Incoming SIGUSR2 signal */
      printf("Child: Received %d\n", SIGUSR2);
      break;
    default: break;  /* Should never get this case */
  }
}

int main(void) {
  int child_pid, parent_pid, status;

  if (signal(SIGUSR1, sig_handler) != SIG_ERR)
    printf("Parent: Handler created for %d\n", SIGUSR1);
  if (signal(SIGUSR2, sig_handler) != SIG_ERR)
    printf("Parent: Handler created for %d\n", SIGUSR2);

  parent_pid = getpid();
  if ((child_pid = fork()) == 0) {
    kill(parent_pid, SIGUSR1);  /* Child raising SIGUSR1 */
    for (;;) pause();           /* Child busy waiting for a signal */
  } else {
    kill(child_pid, SIGUSR2);   /* Parent raising SIGUSR2 */
    pause();                    /* Wait for SIGUSR1 from child */
    printf("Terminating child ...");
    kill(child_pid, SIGTERM);   /* Parent raising SIGTERM */
    wait(&status);              /* Parent waiting for child termination */
    printf("done\n");
  }

  return EXIT_SUCCESS;
}
