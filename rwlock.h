/*
 *  Project: Read-Write Lock for C
 *  File:   rwlock.h
 *  Author: Jason Papapanagiotakis
 *  Github:	https://github.com/JasonPap/ReadWriteLock
 */
#ifndef RWLOCK_H
#define	RWLOCK_H

#ifdef	__cplusplus
extern "C" {
#endif
typedef struct ReadWriteLock_s* ReadWriteLock;

/* This function allocates and initializes the read-write lock.
 * Return values:
 *      [*] On success,  0 is returned
 *      [*] On failure,  -1 is returned
 */
int rwl_init(ReadWriteLock*);

/* This function deletes the read-write lock object and release
 * the resources allocated.
 * Return values:
 *      [*] On success,  0 is returned
 *      [*] On failure,  -1 is returned
 */
int rwl_destroy(ReadWriteLock*);

/* This function lock the read-write lock for write usage.
 * Will return when the lock is obtained.
 * Return values:
 *      [*] On success,  0 is returned
 *      [*] On failure,  -1 is returned
 */
int rwl_writeLock(ReadWriteLock);

/* This function release the read-write lock from write usage.
 * Will return when the lock is released.
 * Return values:
 *      [*] On success,  0 is returned
 *      [*] On failure,  -1 is returned
 */
int rwl_writeUnlock(ReadWriteLock);

/* This function lock the read-write lock for read usage.
 * Will return when the lock is obtained.
 * Return values:
 *      [*] On success,  0 is returned
 *      [*] On failure,  -1 is returned
 */
int rwl_readLock(ReadWriteLock);

/* This function release the read-write lock from read usage.
 * Will return when the lock is released.
 * Return values:
 *      [*] On success,  0 is returned
 *      [*] On failure,  -1 is returned
 */
int rwl_readUnlock(ReadWriteLock);


#ifdef	__cplusplus
}
#endif

#endif	/* RWLOCK_H */
