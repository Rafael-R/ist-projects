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

#if defined(MUTEX)
    #define LOCK_DECLARATION pthread_mutex_t
    #define LOCK_INIT pthread_mutex_init
    #define LOCK_DESTROY pthread_mutex_destroy
    #define LOCK_UNLOCK pthread_mutex_unlock
#elif defined(RWLOCK)
    #define LOCK_DECLARATION pthread_rwlock_t
    #define LOCK_INIT pthread_rwlock_init
    #define LOCK_DESTROY pthread_rwlock_destroy
    #define LOCK_UNLOCK pthread_rwlock_unlock
#else
    #define NOSYNC 1
#endif

char* inputFile;
char* outputFile;
int numberThreads;
tecnicofs* fs;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

#ifndef NOSYNC
pthread_mutex_t commandsAccess;
LOCK_DECLARATION directoryAccess;
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

    #ifndef NOSYNC
        if((pthread_mutex_init(&commandsAccess, NULL)) != 0 &&
           (LOCK_INIT(&directoryAccess, NULL)) != 0) {
            fprintf(stderr, "Error: initializing lock\n");
            exit(EXIT_FAILURE);
        }
    #endif

    while(numberCommands > 0){

        #ifndef NOSYNC
            pthread_mutex_lock(&commandsAccess);
        #endif

        const char* command = removeCommand();
        if (command == NULL){
            #ifndef NOSYNC
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

        #ifndef NOSYNC
            pthread_mutex_unlock(&commandsAccess);
        #endif

        #ifdef MUTEX
            pthread_mutex_lock(&directoryAccess);
        #elif WRLOCK
            pthread_rwlock_rdlock(&directoryAccess);
            pthread_rwlock_wrlock(&directoryAccess);
        #endif

        int searchResult;
        switch (token) {
            case 'c':
                create(fs, name, iNumber);
                #ifndef NOSYNC
                    LOCK_UNLOCK(&directoryAccess);
                #endif
                break;
            case 'l':
                searchResult = lookup(fs, name);
                if(!searchResult)
                    printf("%s not found\n", name);
                else
                    printf("%s found with inumber %d\n", name, searchResult);
                #ifndef NOSYNC
                    LOCK_UNLOCK(&directoryAccess);
                #endif
                break;
            case 'd':
                delete(fs, name);
                #ifndef NOSYNC
                    LOCK_UNLOCK(&directoryAccess);
                #endif
                break;
            default: { /* error */
                fprintf(stderr, "Error: command to apply\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    #ifndef NOSYNC
        if((pthread_mutex_destroy(&commandsAccess)) != 0 &&
           (LOCK_DESTROY(&directoryAccess)) != 0) {
            fprintf(stderr, "Error: destroying lock\n");
            exit(EXIT_FAILURE);
        }
        pthread_exit(NULL);
    #else
        return NULL;
    #endif
}


int main(int argc, char* argv[]) {

    parseArgs(argc, argv);

    fs = new_tecnicofs();
    processInput();

    struct timeval start, end;

    #ifndef NOSYNC
        pthread_t* threads = (pthread_t*) malloc(numberThreads * sizeof(pthread_t));
    #endif

    gettimeofday(&start, NULL);

    #ifndef NOSYNC
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
        applyCommands(NULL);
    #endif

    gettimeofday(&end, NULL);

    float duration = (end.tv_sec - start.tv_sec) * 1000.0 +
                     (end.tv_usec - start.tv_usec) / 1000.0;

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
