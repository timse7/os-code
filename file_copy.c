#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

#define BUF_SIZE 4096
#define OUTPUT_MODE 0700

int main(int argc, char *argv[]) {
  int in_fd, out_fd, rd_count, wt_count;
  char buffer[BUF_SIZE];

  if (argc != 3) {
    fprintf(stderr, "usage: %s <source> <dest>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  in_fd = open(argv[1], O_RDONLY);
  if (in_fd < 0) {
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  out_fd = creat(argv[2], OUTPUT_MODE);
  if (out_fd < 0) {
    perror(argv[2]);
    exit(EXIT_FAILURE);
  }

  while (1) {
    rd_count = read(in_fd, buffer, BUF_SIZE);
    if (rd_count <= 0)
      break;
    wt_count = write(out_fd, buffer, rd_count);
    if (wt_count <= 0) {
      perror("write");
      exit(EXIT_FAILURE);
    }
  }

  close(in_fd);
  close(out_fd);
  if (rd_count == 0)
    exit(EXIT_SUCCESS);
  else {
    perror("read");
    exit(EXIT_FAILURE);
  }
}
