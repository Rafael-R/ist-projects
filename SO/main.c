#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <sys/time.h>
#include "fs.h"

#define MAX_COMMANDS 150000
#define MAX_INPUT_SIZE 100

char* inputFile;
char* outputFile;
int numberThreads;
tecnicofs* fs;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

#if !defined(NOSYNC)
    pthread_mutex_t commandsAccess;
    #if defined(MUTEX)
        pthread_mutex_t directoryAccess;
        #define LOCK_INIT(lock) pthread_mutex_init(&lock, NULL)
        #define LOCK_WRLOCK(lock) pthread_mutex_lock(&lock)         // Sendo que o mutex e' bloqueado
        #define LOCK_RDLOCK(lock) pthread_mutex_lock(&lock)         // sempre da mesma forma, ao 
        #define LOCK_UNLOCK(lock) pthread_mutex_unlock(&lock)       // contrario do rwlock, tem de
        #define LOCK_DESTROY(lock) pthread_mutex_destroy(&lock)     // definido duas vezes
    #elif defined(RWLOCK)
        pthread_rwlock_t directoryAccess;
        #define LOCK_INIT(lock) pthread_rwlock_init(&lock, NULL)
        #define LOCK_WRLOCK(lock) pthread_rwlock_wrlock(&lock)
        #define LOCK_RDLOCK(lock) pthread_rwlock_rdlock(&lock)
        #define LOCK_UNLOCK(lock) pthread_rwlock_unlock(&lock)
        #define LOCK_DESTROY(lock) pthread_rwlock_destroy(&lock)
    #else
        #define NOSYNC
    #endif
#endif

static void displayUsage (const char* appName) {
    printf("Usage: %s\n", appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (long argc, char* const argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Invalid format:\n");
        displayUsage(argv[0]);
    }
    inputFile = argv[1];
    outputFile = argv[2];
    numberThreads = atoi(argv[3]);
}

int insertCommand(char* data) {
    if(numberCommands != MAX_COMMANDS) {
        strcpy(inputCommands[numberCommands++], data);
        return 1;
    }
    return 0;
}

char* removeCommand() {
    if(numberCommands > 0){
        numberCommands--;
        return inputCommands[headQueue++];  
    }
    return NULL;
}

void errorParse() {
    fprintf(stderr, "Error: command invalid\n");
    exit(EXIT_FAILURE);
}

void processInput() {
    char line[MAX_INPUT_SIZE];

    FILE* fptr;
    if ((fptr = fopen(inputFile, "r")) == NULL) {
        fprintf(stderr, "Error: invalid input file\n");
        exit(EXIT_FAILURE);
    }

    while (fgets(line, sizeof(line)/sizeof(char), fptr)) {
        char token;
        char name[MAX_INPUT_SIZE];

        int numTokens = sscanf(line, "%c %s", &token, name);

        /* perform minimal validation */
        if (numTokens < 1) {
            continue;
        }
        switch (token) {
            case 'c':
            case 'l':
            case 'd':
                if(numTokens != 2)
                    errorParse();
                if(insertCommand(line))
                    break;
                return;
            case '#':
                break;
            default: { /* error */
                errorParse();
            }
        }
    }

    fclose(fptr);
}

void* applyCommands(void* arg){

    #if !defined(NOSYNC)
        if((pthread_mutex_init(&commandsAccess, NULL)) != 0 &&
           (LOCK_INIT(directoryAccess)) != 0) {
            fprintf(stderr, "Error: initializing lock\n");
            exit(EXIT_FAILURE);
        }
    #endif

    while(numberCommands > 0){

        #if !defined(NOSYNC)
            pthread_mutex_lock(&commandsAccess);
        #endif

        const char* command = removeCommand();
        if (command == NULL){
            #if !defined(NOSYNC)
                pthread_mutex_unlock(&commandsAccess);
            #endif
            continue;
        }

        char token;
        char name[MAX_INPUT_SIZE];
        int numTokens = sscanf(command, "%c %s", &token, name);
        
        if (numTokens != 2) {
            fprintf(stderr, "Error: invalid command in Queue\n");
            exit(EXIT_FAILURE);
        }
        
        int iNumber;
        if (token == 'c') {
            iNumber = obtainNewInumber(fs);
        }

        #if !defined(NOSYNC)                            // O acesso ao vetor de comandos
            pthread_mutex_unlock(&commandsAccess);      // e ao iNumber e' sempre protegido
        #endif                                          // por um mutex.

        int searchResult;
        switch (token) {
            case 'c':
                #if !defined(NOSYNC) 
                    LOCK_WRLOCK(directoryAccess);
                #endif

                create(fs, name, iNumber);

                #if !defined(NOSYNC)
                    LOCK_UNLOCK(directoryAccess);
                #endif
                break;
            case 'l':
                #if !defined(NOSYNC) 
                    LOCK_RDLOCK(directoryAccess);
                #endif

                searchResult = lookup(fs, name);
                if(!searchResult)
                    printf("%s not found\n", name);
                else
                    printf("%s found with inumber %d\n", name, searchResult);

                #if !defined(NOSYNC)
                    LOCK_UNLOCK(directoryAccess);
                #endif
                break;
            case 'd':
                #if !defined(NOSYNC) 
                    LOCK_WRLOCK(directoryAccess);
                #endif

                delete(fs, name);

                #if !defined(NOSYNC)
                    LOCK_UNLOCK(directoryAccess);
                #endif
                break;
            default: { /* error */
                fprintf(stderr, "Error: command to apply\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    #if !defined(NOSYNC)
        if((pthread_mutex_destroy(&commandsAccess)) != 0 &&
           (LOCK_DESTROY(directoryAccess)) != 0) {
            fprintf(stderr, "Error: initializing lock\n");
            exit(EXIT_FAILURE);
        }
        pthread_exit(NULL);
    #else
        return NULL;    // Return normal em caso de execucao sequencial
    #endif
}

int main(int argc, char* argv[]) {

    parseArgs(argc, argv);

    fs = new_tecnicofs();
    processInput();

    struct timeval start, end;

    #if !defined(NOSYNC)
        pthread_t* threads = (pthread_t*) malloc(numberThreads * sizeof(pthread_t));
    #endif

    gettimeofday(&start, NULL);

    #if !defined(NOSYNC)
        for (int i = 0; i < numberThreads; i++) {
            if((pthread_create(&threads[i], NULL, applyCommands, NULL)) != 0) {
                fprintf(stderr, "Error: creating thread\n");
                exit(EXIT_FAILURE);
            }
        }
        for (int i = 0; i < numberThreads; i++) {
            if((pthread_join(threads[i], NULL)) != 0) {
                fprintf(stderr, "Error: joining thread\n");
                exit(EXIT_FAILURE);
            }
        }
    #else
        applyCommands(NULL);    // Execucao sequencial
    #endif

    gettimeofday(&end, NULL);

    double duration = (end.tv_sec - start.tv_sec) +
                      ((end.tv_usec - start.tv_usec) / 1000000.0);

    printf("TecnicoFS completed in %.4f seconds.\n", duration);

    FILE* fptr;
    if ((fptr = fopen(outputFile, "w")) == NULL) {
        fprintf(stderr, "Error: invalid output file\n");
        exit(EXIT_FAILURE);
    }
    print_tecnicofs_tree(fptr, fs);
    fclose(fptr);

    free_tecnicofs(fs);
    exit(EXIT_SUCCESS);
}
