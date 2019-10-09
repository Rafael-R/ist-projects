#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <ctype.h>
#include <pthread.h>
#include <time.h>
#include "fs.h"

#define MAX_COMMANDS 150000
#define MAX_INPUT_SIZE 100

#if defined(MUTEX)
    #define VAR 1
#elif defined(RWLOCK)
    #define VAR 2
#else
    #define VAR 0
#endif

char* inputFile;
char* outputFile;
int numberThreads;
tecnicofs* fs;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

pthread_mutex_t mutex1;
pthread_mutex_t mutex2;
pthread_rwlock_t rwlock;

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

    if((pthread_mutex_init(&mutex1, NULL)) &&
       (pthread_mutex_init(&mutex2, NULL))) {
        fprintf(stderr, "Error: initializing lock\n");
        exit(EXIT_FAILURE);
    }
    /*if((pthread_rwlock_init(&rwlock, NULL)) != 0) {
        fprintf(stderr, "Error: initializing lock\n");
        exit(EXIT_FAILURE);
    }*/

    while(numberCommands > 0){
        pthread_mutex_lock(&mutex1);
        //pthread_rwlock_rdlock(&rwlock);
        const char* command = removeCommand();
        if (command == NULL){
            pthread_mutex_unlock(&mutex1);
            //pthread_rwlock_unlock(&rwlock);
            continue;
        }

        char token;
        char name[MAX_INPUT_SIZE];
        int numTokens = sscanf(command, "%c %s", &token, name);
        
        if (numTokens != 2) {
            fprintf(stderr, "Error: invalid command in Queue\n");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_unlock(&mutex1);
        pthread_mutex_lock(&mutex2);

        int searchResult;
        int iNumber;
        switch (token) {
            case 'c':
                iNumber = obtainNewInumber(fs);
                create(fs, name, iNumber);
                pthread_mutex_unlock(&mutex2);
                //pthread_rwlock_unlock(&rwlock);
                break;
            case 'l':
                searchResult = lookup(fs, name);
                if(!searchResult)
                    printf("%s not found\n", name);
                else
                    printf("%s found with inumber %d\n", name, searchResult);
                pthread_mutex_unlock(&mutex2);
                //pthread_rwlock_unlock(&rwlock);
                break;
            case 'd':
                delete(fs, name);
                pthread_mutex_unlock(&mutex2);
                //pthread_rwlock_unlock(&rwlock);
                break;
            default: { /* error */
                fprintf(stderr, "Error: command to apply\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if((pthread_mutex_destroy(&mutex1)) &&
       (pthread_mutex_destroy(&mutex2))) {
        fprintf(stderr, "Error: destroying lock\n");
        exit(EXIT_FAILURE);
    }
    /*if((pthread_rwlock_destroy(&rwlock)) != 0) {
        fprintf(stderr, "Error: destroying lock\n");
        exit(EXIT_FAILURE);
    }*/

    pthread_exit(NULL);
}


int main(int argc, char* argv[]) {

    int x = VAR;
    printf("%d\n", x);

    parseArgs(argc, argv);

    fs = new_tecnicofs();
    processInput();

    clock_t start = clock();
    //---------------------------------

    pthread_t *threads = malloc(numberThreads * sizeof(pthread_t));

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
    
    //---------------------------------
    clock_t end = clock();
    float duration = (float) (end - start) / CLOCKS_PER_SEC;

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
