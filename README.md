# A portable Read-Write lock in C
This project aims to create a easy to use and portable read-write lock for C.
This is implemented by using POSIX semaphores and "hide" the struct declaration to avoid missuse of the API provided.

This lock is **readers-prefered**, meaning that a writer will have to wait for every reader to finish before being able to acquire
the lock. For more information on that see [here](http://en.wikipedia.org/wiki/Readers%E2%80%93writers_problem#The_first_readers-writers_problem).

I'm currently trying to make the destruction of the lock thread-safe. Until that is achieved the user of the API is responsible
for releasing any lock before calling `rwl_destroy()`.
