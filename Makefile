targets = badcnt goodcnt goodcnt_mutex prodcons_mutex prodcons_sem simple_pipe shm
 
CC = /usr/bin/gcc
CFLAGS = -Wall -pthread

src = $(wildcard *.c)
hdr = $(wildcard *.h)
obj = $(src:.c=.o)

.PHONY: all
all: $(targets)

.PHONY: $(src)
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ ./$^

.PHONY: format 
format: $(hdr) $(src)
	@clang-format -i -fallback-style=WebKit -style=file $^  # uses custom `.clang-format`; WebKit as fallback
	@echo "Source files formatted."

.PHONY: clean
clean:
	@rm -f $(obj) $(main) $(targets)
