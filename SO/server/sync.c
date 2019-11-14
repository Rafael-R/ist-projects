#include "sync.h"
#include <stdio.h>
#include <stdlib.h>

void sync_init(LOCK_TYPE* sync){
    int status = LOCK_INIT(sync);
    if(status != 0){
        fprintf(stderr, "Error: initializing lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_destroy(LOCK_TYPE* sync){
    int status = LOCK_DESTROY(sync);
    if(status != 0){
        fprintf(stderr, "Error: destroying lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_wrlock(LOCK_TYPE* sync){
    int status = LOCK_WRLOCK(sync);
    if(status != 0){
        fprintf(stderr, "Error: write locking lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_rdlock(LOCK_TYPE* sync){
    int status = LOCK_RDLOCK(sync);
    if(status != 0){
        fprintf(stderr, "Error: read locking lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_unlock(LOCK_TYPE* sync){
    int status = LOCK_UNLOCK(sync);
    if(status != 0){
        fprintf(stderr, "Error: unlocking lock\n");
        exit(EXIT_FAILURE);
    }
}

void thread_create(pthread_t *thread, void *(*function) (void *), void* arg) {
    int status = pthread_create(thread, NULL, function, arg);
    if(status != 0){
        fprintf(stderr, "Error: creating thread\n");
        exit(EXIT_FAILURE);
    }
}

void thread_join(pthread_t thread) {
    int status = pthread_join(thread, NULL);
    if(status != 0){
        fprintf(stderr, "Error: joining thread\n");
        exit(EXIT_FAILURE);
    }
}

void mutex_init(pthread_mutex_t* mutex) {
    int status = pthread_mutex_init(mutex, NULL);
    if(status != 0){
        fprintf(stderr, "Error: initializing mutex\n");
        exit(EXIT_FAILURE);
    }
}

void mutex_lock(pthread_mutex_t* mutex) {
    int status = pthread_mutex_lock(mutex);
    if(status != 0){
        fprintf(stderr, "Error: locking mutex\n");
        exit(EXIT_FAILURE);
    }
}

void mutex_unlock(pthread_mutex_t* mutex) {
    int status = pthread_mutex_unlock(mutex);
    if(status != 0){
        fprintf(stderr, "Error: unlocking mutex\n");
        exit(EXIT_FAILURE);
    }
}

void mutex_destroy(pthread_mutex_t* mutex) {
    int status = pthread_mutex_destroy(mutex);
    if(status != 0){
        fprintf(stderr, "Error: destroying mutex\n");
        exit(EXIT_FAILURE);
    }
}

void semaphore_init(sem_t* semaphore, unsigned int value){
    int status = sem_init(semaphore, 0, value);
    if(status != 0){
        fprintf(stderr, "Error: initializing semaphore\n");
        exit(EXIT_FAILURE);
    }
}

void semaphore_destroy(sem_t* semaphore){
    int status = sem_destroy(semaphore);
    if(status != 0){
        fprintf(stderr, "Error: destroying semaphore\n");
        exit(EXIT_FAILURE);
    }
}

void semaphore_wait(sem_t* semaphore){
    int status = sem_wait(semaphore);
    if(status != 0){
        fprintf(stderr, "Error: locking semaphore\n");
        exit(EXIT_FAILURE);
    }
}

void semaphore_post(sem_t* semaphore){
    int status = sem_post(semaphore);
    if(status != 0){
        fprintf(stderr, "Error: unlocking semaphore\n");
        exit(EXIT_FAILURE);
    }
}

int nop(void* pointer){
    (void)pointer;
    return 0;
}
