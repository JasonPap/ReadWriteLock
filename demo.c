/*
 *  Project: Read-Write Lock for C
 *  File:   demo.c
 *  Author: Jason Papapanagiotakis
 *  Github: https://github.com/JasonPap/ReadWriteLock
 */
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "rwlock.h"

void* read_operation(void* vlock)
{
    ReadWriteLock lock = (ReadWriteLock)vlock;
    pthread_t id = pthread_self();
    printf("Thread: %d - started, I'm a reader.\n", id);
    rwl_readLock(lock);
    printf("Thread: %d - locked for read.\n", id);
    sleep(5);
    rwl_readUnlock(lock);
    printf("Thread: %d - unlocked from read locked.\n", id);
    return 0;
}

void* write_operation(void* vlock)
{
    sleep(1);
    ReadWriteLock lock = (ReadWriteLock)vlock;
    pthread_t id = pthread_self();
    printf("Thread: %d - started, I'm a writer.\n", id);
    rwl_writeLock(lock);
    printf("Thread: %d - locked for write.\n", id);
    sleep(1);
    rwl_writeUnlock(lock);
    printf("Thread: %d - unlocked from write locked.\n", id);
    return 0;
}

int main(void)
{
    pthread_t reader1, reader2, writer1;
    ReadWriteLock lock;
    if(rwl_init(&lock) == -1)                    //initialization of the lock
        return -1;
    printf("lock initialized.\n");
    pthread_create(&reader1, NULL, read_operation, lock);
    pthread_create(&reader2, NULL, read_operation, lock);
    pthread_create(&writer1, NULL, write_operation, lock);
    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);
    pthread_join(writer1, NULL);
    return 0;
}
