#include "sync.h"
#include <stdio.h>
#include <stdlib.h>

void sync_init(LOCK_TYPE* sync){
    if(LOCK_INIT(sync) != 0){
        fprintf(stderr, "Error: initializing lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_destroy(LOCK_TYPE* sync){
    if(LOCK_DESTROY(sync) != 0){
        fprintf(stderr, "Error: initializing lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_wrlock(LOCK_TYPE* sync){
    if(LOCK_WRLOCK(sync) != 0){
        fprintf(stderr, "Error: write locking lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_rdlock(LOCK_TYPE* sync){
    if(LOCK_RDLOCK(sync) != 0){
        fprintf(stderr, "Error: read locking lock\n");
        exit(EXIT_FAILURE);
    }
}

void sync_unlock(LOCK_TYPE* sync){
    if(LOCK_UNLOCK(sync) != 0){
        fprintf(stderr, "Error: unlocking lock\n");
        exit(EXIT_FAILURE);
    }
}

void thread_create(pthread_t *thread, void *(*function) (void *)) {
    if((pthread_create(thread, NULL, function, NULL)) != 0) {
        fprintf(stderr, "Error: creating thread\n");
        exit(EXIT_FAILURE);
    }
}

void thread_join(pthread_t thread) {
    if((pthread_join(thread, NULL)) != 0) {
        fprintf(stderr, "Error: joining thread\n");
        exit(EXIT_FAILURE);
    }
}

void mutex_init(pthread_mutex_t* mutex){
    #if !defined(NOSYNC)
        if(pthread_mutex_init(mutex, NULL) != 0){
            fprintf(stderr, "Error: initializing mutex\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

void mutex_destroy(pthread_mutex_t* mutex){
    #if !defined(NOSYNC)
        if(pthread_mutex_destroy(mutex) != 0){
            fprintf(stderr, "Error: destroying mutex\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

void mutex_lock(pthread_mutex_t* mutex){
    #if !defined(NOSYNC)
        if(pthread_mutex_lock(mutex) != 0){
            fprintf(stderr, "Error: locking mutex\n");
            exit(EXIT_FAILURE);
        }
    #endif
}

void mutex_unlock(pthread_mutex_t* mutex){
    #if !defined(NOSYNC)
        if(pthread_mutex_unlock(mutex) != 0){
            fprintf(stderr, "Error: unlocking mutex\n");
            exit(EXIT_FAILURE);
        }
     #endif
}

int nop(void* pointer){
    (void) pointer;
    return 0;
}
