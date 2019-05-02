# OS Code
Code for my operating system lecture at University of Klagenfurt (https://itec.aau.at/)

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

