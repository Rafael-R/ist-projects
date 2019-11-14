#ifndef SYNC_H
#define SYNC_H

#include <pthread.h>
#include <semaphore.h> 

#if defined(MUTEX)
    #define LOCK_TYPE           pthread_mutex_t
    #define LOCK_INIT(lock)     pthread_mutex_init(lock, NULL)
    #define LOCK_WRLOCK(lock)   pthread_mutex_lock(lock)
    #define LOCK_RDLOCK(lock)   pthread_mutex_lock(lock)
    #define LOCK_UNLOCK(lock)   pthread_mutex_unlock(lock)
    #define LOCK_DESTROY(lock)  pthread_mutex_destroy(lock)
#elif defined(RWLOCK)
    #define LOCK_TYPE           pthread_rwlock_t
    #define LOCK_INIT(lock)     pthread_rwlock_init(lock, NULL)
    #define LOCK_WRLOCK(lock)   pthread_rwlock_wrlock(lock)
    #define LOCK_RDLOCK(lock)   pthread_rwlock_rdlock(lock)
    #define LOCK_UNLOCK(lock)   pthread_rwlock_unlock(lock)
    #define LOCK_DESTROY(lock)  pthread_rwlock_destroy(lock)
#else
    #define LOCK_TYPE           void*
    #define LOCK_INIT(lock)     nop(lock)
    #define LOCK_WRLOCK(lock)   nop(lock)
    #define LOCK_RDLOCK(lock)   nop(lock)
    #define LOCK_UNLOCK(lock)   nop(lock)
    #define LOCK_DESTROY(lock)  nop(lock)
#endif

void sync_init(LOCK_TYPE * sync);
void sync_destroy(LOCK_TYPE * sync);
void sync_wrlock(LOCK_TYPE * sync);
void sync_rdlock(LOCK_TYPE * sync);
void sync_unlock(LOCK_TYPE * sync);

void thread_create(pthread_t *thread, void *(*function) (void *), void* arg);
void thread_join(pthread_t thread);

void mutex_init(pthread_mutex_t* mutex);
void mutex_lock(pthread_mutex_t* mutex);
void mutex_unlock(pthread_mutex_t* mutex);
void mutex_destroy(pthread_mutex_t* mutex);

void semaphore_init(sem_t* semaphore, unsigned int value);
void semaphore_wait(sem_t* semaphore);
void semaphore_post(sem_t* semaphore);
void semaphore_destroy(sem_t* semaphore);

int nop(void* pointer);

#endif /* SYNC_H */
