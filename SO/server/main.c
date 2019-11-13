#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "fs.h"
#include "constants.h"
#include "timer.h"

char* socketName;
char* outputFile;
int numberBuckets;
tecnicofs* fs;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

pthread_mutex_t commandLock;
pthread_mutex_t insertLock;
pthread_mutex_t removeLock;
sem_t fullBuffer;
sem_t emptyBuffer;


static void displayUsage (const char* appName) {
    printf("Usage: %s\n", appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (long argc, char* const argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Invalid format:\n");
        displayUsage(argv[0]);
    }
    socketName = argv[1];
    outputFile = argv[2];

    #if defined(MUTEX) || defined(RWLOCK)
        numberBuckets = atoi(argv[3]);
        if (numberBuckets < 1) {
            fprintf(stderr, "Error: invalid number of buckets\n");
            exit(EXIT_FAILURE);
        }
    #else
        numberBuckets = 1;
    #endif
}

void insertCommand(char* data) {
    semaphore_wait(&emptyBuffer);
    mutex_lock(&insertLock);

    strcpy(inputCommands[numberCommands], data);
    numberCommands = (numberCommands + 1) % MAX_COMMANDS;

    mutex_unlock(&insertLock);
    semaphore_post(&fullBuffer);
}

char* removeCommand() {
    semaphore_wait(&fullBuffer);
    mutex_lock(&removeLock);

    char* command = inputCommands[headQueue];
    headQueue = (headQueue + 1) % MAX_COMMANDS;

    mutex_unlock(&removeLock);
    semaphore_post(&emptyBuffer);
    return command;
}

void errorParse(int lineNumber){
    fprintf(stderr, "Error: line %d invalid\n", lineNumber);
    exit(EXIT_FAILURE);
}

FILE* openFile(char* filename, const char* mode) {
    FILE* fptr;
    fptr = fopen(filename, mode);
    if(!fptr){
        fprintf(stderr, "Error: Opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }
    return fptr;
}

void* processInput() {
    FILE* input = openFile(inputFile, "r");
    char line[MAX_INPUT_SIZE];
    int lineNumber = 0;

    while(fgets(line, sizeof(line)/sizeof(char), input)) {
        lineNumber++;

        char token;
        char name[MAX_INPUT_SIZE], newName[MAX_INPUT_SIZE];

        int numTokens = sscanf(line, "%c %s %s", &token, name, newName);

        /* perform minimal validation */
        if (numTokens < 1) {
            continue;
        }
        switch (token) {
            case 'c':
            case 'l':
            case 'd':
                if(numTokens != 2) {
                    errorParse(lineNumber);
                } else {
                    insertCommand(line);
                    break;
                }
            case 'r':
                if(numTokens != 3) {
                    errorParse(lineNumber);
                } else {
                    insertCommand(line);
                    break;
                }
            case '#':
                break;
            default: { /* error */
                errorParse(lineNumber);
            }
        }
    }
    fclose(input);
    for (int i = 0; i < numberThreads; i++) {
        insertCommand(EXIT_COMMAND);
    }
    pthread_exit(NULL);
}

void* applyCommands(){
    while(1){
        mutex_lock(&commandLock);
        const char* command = removeCommand();

        char token;
        char name[MAX_INPUT_SIZE], newName[MAX_INPUT_SIZE];
        sscanf(command, "%c %s %s", &token, name, newName);

        int iNumber;
        switch (token) {
            case 'c':
                iNumber = obtainNewInumber(fs);
                mutex_unlock(&commandLock);
                create_file(fs, name, iNumber);
                break;
            case 'l':
                mutex_unlock(&commandLock);
                int searchResult = lookup_file(fs, name);
                if(!searchResult)
                    printf("%s not found\n", name);
                else
                    printf("%s found with inumber %d\n", name, searchResult);
                break;
            case 'd':
                mutex_unlock(&commandLock);
                delete_file(fs, name);
                break;
            case 'r':
                mutex_unlock(&commandLock);
                rename_file(fs, name, newName);
                break;
            case 'q':
                mutex_unlock(&commandLock);
                pthread_exit(NULL);
            default: { /* error */
                mutex_unlock(&commandLock);
                fprintf(stderr, "Error: command to apply\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


int main(int argc, char* argv[]) {

    parseArgs(argc, argv);

    fs = new_tecnicofs(numberBuckets);

    pthread_t* threads = (pthread_t*) malloc((numberThreads + 1) * sizeof(pthread_t));

    mutex_init(&commandLock);
    mutex_init(&insertLock);
    mutex_init(&removeLock);
    semaphore_init(&emptyBuffer, MAX_COMMANDS);
    semaphore_init(&fullBuffer, 0);

    TIMER start, end;

    TIMER_READ(start);

    thread_create(&threads[0], processInput);

    for (int i = 1; i < numberThreads + 1; i++) {
        thread_create(&threads[i], applyCommands);
    }
    for (int i = 0; i < numberThreads + 1; i++) {
        thread_join(threads[i]);
    }

    TIMER_READ(end);

    semaphore_destroy(&emptyBuffer);
    semaphore_destroy(&fullBuffer);
    mutex_destroy(&removeLock);
    mutex_destroy(&insertLock);
    mutex_destroy(&commandLock);

    free(threads);

    printf("TecnicoFS completed in %.4f seconds.\n", TIMER_GET_DURATION(start, end));

    FILE* output = openFile(outputFile, "w");
    print_tecnicofs_tree(output, fs);
    fclose(output);

    free_tecnicofs(fs);
    exit(EXIT_SUCCESS);
}
