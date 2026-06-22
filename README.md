# OS Code
Code for my operating system lecture at University of Klagenfurt (https://itec.aau.at/)

## Build

```bash
make        # build all targets
make clean  # remove build artifacts
make format # auto-format source files (requires clang-format)
```

## Threading

| File | Description |
|------|-------------|
| `badcnt.c` | Traditional race condition demo based on [ostep-code](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/threads-intro) |
| `goodcnt.c` | Race-free counter using named POSIX semaphores (macOS-compatible) |
| `goodcnt_mutex.c` | Race-free counter using pthread mutex |
| `thread-shared-vars.c` | Example of threads accessing another thread's stack |
| `runner.c` | Simple program summing integers using pthreads |

## Synchronization

| File | Description |
|------|-------------|
| `prodcons_mutex.c` | Producer-consumer problem using pthread mutex and condition variables |
| `prodcons_sem.c` | Producer-consumer problem using named POSIX semaphores (macOS-compatible) |

## IPC

| File | Description |
|------|-------------|
| `simple_pipe.c` | Half-duplex pipe: parent writes to child |
| `pipe_duplex.c` | Duplex pipe: parent sends integer, child transforms and replies |
| `pipe_named.c` | Named pipe (FIFO): same transfer as simple_pipe but via a filesystem FIFO created with `mkfifo()` |
| `shm.c` | Shared memory example with race condition (System V) |
| `sysV-msg.c` | System V message queue example |

## Processes

| File | Description |
|------|-------------|
| `ctshell.c` | A stripped-down shell named 'ctsh' ;) |
| `signals.c` | Signal handling example: parent and child exchange SIGUSR1/SIGUSR2 via explicit handlers, parent terminates child with SIGTERM |

## File I/O

| File | Description |
|------|-------------|
| `file_copy.c` | File copy using read/write syscalls |
| `file_copy_mmap.c` | File copy using mmap |
