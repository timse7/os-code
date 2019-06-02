# OS Code
Code for my operating system lecture at University of Klagenfurt (https://itec.aau.at/)

## ctshell.c
A stripped-down shell named 'ctsh' ;)

## simple_pipe.c
A simple example for fork + pipe

## shm.c
A simple shared memory example with race condition

## sysV-msg.c
A simple System V message queues example

## runner.c
A simple program using pthreads

## thread-shared-vars.c
Example of threads accessing another thread’s stack

## badcnt.c
Traditional badcnt.c program based on [ostep-code](https://github.com/remzi-arpacidusseau/ostep-code/tree/master/threads-intro)

## goodcnt.c
Improved version of badcnt.c using semaphore.h. It uses named semaphores as I have programmed it on macOS which only supports named sempahores.

## goodcnt_mutex.c
Improved version of badcnt.c using phtread.h mutex

## prodcons_sem.c
Producer-consumer problem using semaphore.h. It uses named semaphores as I have programmed it on macOS which only support named semaphores.

## prodcons_mutex.c
Producer-consumer problem using phtread.h mutex and condition variables

## file_copy.c
Example program using file system calls
