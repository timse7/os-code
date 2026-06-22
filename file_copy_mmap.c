#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define OUTPUT_MODE 0700

static size_t getFilesize(const char *filename) {
  struct stat st;
  if (stat(filename, &st) < 0) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  return st.st_size;
}

int main(int argc, char *argv[]) {
  int in_fd, out_fd, wt_count, rc;
  size_t filesize;
  void *mmappedData;

  if (argc != 3) {
    fprintf(stderr, "usage: %s <source> <dest>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  filesize = getFilesize(argv[1]);

  in_fd = open(argv[1], O_RDONLY);
  if (in_fd < 0) {
    perror(argv[1]);
    exit(EXIT_FAILURE);
  }

  mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, in_fd, 0);
  if (mmappedData == MAP_FAILED) {
    perror("mmap");
    exit(EXIT_FAILURE);
  }

  out_fd = creat(argv[2], OUTPUT_MODE);
  if (out_fd < 0) {
    perror(argv[2]);
    exit(EXIT_FAILURE);
  }

  wt_count = write(out_fd, mmappedData, filesize);
  if (wt_count < 0) {
    perror("write");
    exit(EXIT_FAILURE);
  }

  rc = munmap(mmappedData, filesize);
  if (rc != 0) {
    perror("munmap");
    exit(EXIT_FAILURE);
  }

  close(in_fd);
  close(out_fd);
  exit(EXIT_SUCCESS);
}
