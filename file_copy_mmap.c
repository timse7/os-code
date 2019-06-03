#include <assert.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char *argv[]);
size_t getFilesize(const char *filename);

#define OUTPUT_MODE 0700

int main(int argc, char *argv[]) {
  int in_fd, out_fd, wt_count, rc;
  size_t filesize;
  void *mmappedData;

  if (argc != 3)
    exit(1);

  filesize = getFilesize(argv[1]);

  in_fd = open(argv[1], O_RDONLY);
  if (in_fd < 0)
    exit(2);

  mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, in_fd, 0);
  assert(mmappedData != MAP_FAILED);

  out_fd = creat(argv[2], OUTPUT_MODE);
  if (out_fd < 0)
    exit(3);

  wt_count = write(out_fd, mmappedData, filesize);
  if (wt_count < 0)
	  exit(4);

  rc = munmap(mmappedData, filesize);
  assert(rc == 0);

  close(in_fd);
  close(out_fd);
  exit(0);
}

size_t getFilesize(const char *filename) {
  struct stat st;
  stat(filename, &st);
  return st.st_size;
}
