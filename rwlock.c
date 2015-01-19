/*
 *  Project: Read-Write Lock for C
 *  File:   rwlock.c
 *  Author: Jason Papapanagiotakis
 *  Github:
 */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "rwlock.h"

typedef struct ReadWriteLock
{
    sem_t wrt;
    sem_t mtx;
    sem_t delFlag;
    int readcount;
    int active;
}ReadWriteLock;

//forward declaration
int isActive(ReadWriteLock*);

int rwl_init(ReadWriteLock* lock)
{
    lock = malloc(sizeof(ReadWriteLock));
    if (lock == NULL)
    {
        perror("rwl_init - could not allocate memory for lock\n");
        return -1;
    }
    if (sem_init(&(lock->wrt), 0, 1) == -1)
    {
        perror("rwl_init - could not allocate wrt semaphore\n");
        free(lock);
        return -1;
    }
    if (sem_init(&(lock->mtx), 0, 1) == -1)
    {
        perror("rwl_init - could not allocate mtx semaphore\n");
        sem_destroy(&(lock->wrt));
        free(lock);
        return -1;
    }
    if (sem_init(&(lock->delFlag), 0 , 1) == -1)
    {
        perror("rwl_init - could not allocate delFlag semaphore\n");
        sem_destroy(&(lock->wrt));
        sem_destroy(&(lock->mtx));
        free(lock);
        return -1;
    }

    lock->readcount = 0;
    lock->active = 1;
    return 0;
}

int rwl_destroy(ReadWriteLock* lock)
{
    errno = 0;
    if (sem_trywait(&(lock->wrt)) == -1)
        perror("rwl_destroy - trywait on wrt failed.");
    if ( errno == EAGAIN)
        perror("rwl_destroy - wrt is locked, undefined behaviour.");

    errno = 0;
    if (sem_trywait(&(lock->mtx)) == -1)
        perror("rwl_destroy - trywait on mtx failed.");
    if ( errno == EAGAIN)
        perror("rwl_destroy - mtx is locked, undefined behaviour.");

    if (sem_destroy(&(lock->wrt)) == -1)
        perror("rwl_destroy - destroy wrt failed");
    if (sem_destroy(&(lock->mtx)) == -1)
        perror("rwl_destroy - destroy mtx failed");
    if (sem_destroy(&(lock->delFlag)) == -1)
        perror("rwl_destroy - destroy delFlag failed");

    free(lock);
    lock == NULL;
    return 0;
}

int rwl_writeLock(ReadWriteLock* lock)
{
    if (sem_wait(&(lock->wrt)) == -1)
    {
        perror("rwl_writeLock - wait on wrt failed.");
        return -1;
    }

//    int state = isActive(lock);
//    if(state == -1)     //error occured in isActive() function
//    {
//        sem_post(&(lock->wrt)); //return semaphore to original state
//        return -1;
//    }
//    else if(state == 1) //everything is good
//        return 0;
//    else                //the lock is marked for delete
//    {
//       sem_post(&(lock->wrt)); //return semaphore to original state
//        return -1;
//    }
    return 0;
}

int rwl_writeUnlock(ReadWriteLock* lock)
{
    if (sem_post(&(lock->wrt)) == -1)
    {
        perror("rwl_writeUnlock - post on wrt failed.");
        return -1;
    }
    return 0;
}

int rwl_readLock(ReadWriteLock* lock)
{

    if (sem_wait(&(lock->mtx)) == -1)
    {
        perror("rwl_readLock - wait on mtx failed");
        return -1;
    }

    lock->readcount += 1;
    if (lock->readcount == 1)
        if (sem_wait(&(lock->wrt)) == -1)
        {
            perror("rwl_readLock - wait on wrt failed");
            sem_post(&(lock->mtx)); //return semaphore to original state
            return -1;
        }
    if (sem_post(&(lock->mtx)) == -1)
    {
        perror("rwl_readLock - post on mtx failed, undefined behaviour");
        return -1;
    }
    return 0;
}

int rwl_readUnlock(ReadWriteLock* lock)
{
    if (sem_wait(&(lock->mtx)) == -1)
    {
        perror("rwl_readUnlock - wait on mtx failed");
        return -1;
    }
    lock->readcount -= 1;
    if (lock->readcount == 0)
        if (sem_post(&(lock->wrt)) == -1)
        {
            perror("rwl_readUnlock - post on wrt failed");
            sem_post(&(lock->mtx)); //return semaphore to original state
            return -1;
        }
    if (sem_post(&(lock->mtx)) == -1)
    {
        perror("rwl_readUnlock - post on mtx failed, undefined behaviour");
        return -1;
    }
    return 0;
}

int isActive(ReadWriteLock* lock)
{
    errno = 0;
    if (sem_trywait(&(lock->delFlag)) == -1)
    {
        perror("isActive - trywait on delFlag failed.");
        return -1;
    }
    if ( errno == EAGAIN)
    {//delFlag is down, lock is marked for delete
        perror("isActive - tried to lock but ReadWriteLock was marked for delete");
        return 0;
    }
    return 1;
}
