#include "sync.h"
#include <stdio.h>
#include <stdlib.h>

void sync_init(LOCK_TYPE* sync){
    int ret = LOCK_INIT(sync);
    if(ret != 0){
        fprintf(stderr, "Error: initializing lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_destroy(LOCK_TYPE* sync){
    int ret = LOCK_DESTROY(sync);
    if(ret != 0){
        fprintf(stderr, "Error: destroying lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_wrlock(LOCK_TYPE* sync){
    int ret = LOCK_WRLOCK(sync);
    if(ret != 0){
        fprintf(stderr, "Error: write locking lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_rdlock(LOCK_TYPE* sync){
    int ret = LOCK_RDLOCK(sync);
    if(ret != 0){
        fprintf(stderr, "Error: read locking lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_unlock(LOCK_TYPE* sync){
    int ret = LOCK_UNLOCK(sync);
    if(ret != 0){
        fprintf(stderr, "Error: unlocking lock\n");
        exit(EXIT_FAILURE);
    }
}

void thread_create(pthread_t *thread, void *(*function) (void *), void *arg) {
    int ret = pthread_create(thread, NULL, function, arg);
    if(ret != 0){
        fprintf(stderr, "Error: creating thread\n");
        exit(EXIT_FAILURE);
    }
}

void thread_join(pthread_t thread) {
    int ret = pthread_join(thread, NULL);
    if(ret != 0){
        fprintf(stderr, "Error: joining thread\n");
        exit(EXIT_FAILURE);
    }
}

void semaphore_init(sem_t* semaphore, unsigned int value){
    #if defined(MUTEX) || defined(RWLOCK)
        int ret = sem_init(semaphore, 0, value);
        if(ret != 0){
            fprintf(stderr, "Error: initializing semaphore\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

void semaphore_destroy(sem_t* semaphore){
    #if defined(MUTEX) || defined(RWLOCK)
        int ret = sem_destroy(semaphore);
        if(ret != 0){
            fprintf(stderr, "Error: destroying semaphore\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

void semaphore_wait(sem_t* semaphore){
    #if defined(MUTEX) || defined(RWLOCK)
        int ret = sem_wait(semaphore);
        if(ret != 0){
            fprintf(stderr, "Error: locking semaphore\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

void semaphore_post(sem_t* semaphore){
    #if defined(MUTEX) || defined(RWLOCK)
        int ret = sem_post(semaphore);
        if(ret != 0){
            fprintf(stderr, "Error: unlocking semaphore\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

int nop(void* pointer){
    (void)pointer;
    return 0;
}
