#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "fs.h"
#include "constants.h"
#include "timer.h"

char* inputFile;
char* outputFile;
int numberThreads;
int numberBuckets;
tecnicofs* fs;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

sem_t commandsAccess;
sem_t binarySem;
sem_t fullBuffer;
sem_t emptyBuffer;


static void displayUsage (const char* appName) {
    printf("Usage: %s\n", appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (long argc, char* const argv[]) {
    if (argc != 5) {
        fprintf(stderr, "Invalid format:\n");
        displayUsage(argv[0]);
    }
    inputFile = argv[1];
    outputFile = argv[2];

    #if defined(MUTEX) || defined(RWLOCK)
        numberThreads = atoi(argv[3]);
        numberBuckets = atoi(argv[4]);
    #else
        numberThreads = 1;
        numberBuckets = 1;
    #endif

    if (numberThreads < 1) {
        fprintf(stderr, "Error: invalid number of threads\n");
        exit(EXIT_FAILURE);
    } else if (numberBuckets < 1) {
        fprintf(stderr, "Error: invalid number of buckets\n");
        exit(EXIT_FAILURE);
    }
}

void insertCommand(char* data) {
    if(numberCommands == MAX_COMMANDS) {
        semaphore_wait(&emptyBuffer);
        semaphore_wait(&binarySem);

        strcpy(inputCommands[numberCommands++], data);

        semaphore_post(&binarySem);
        semaphore_post(&fullBuffer);
    } else {
        strcpy(inputCommands[numberCommands++], data);
    }
}

char* removeCommand() {
    if(numberCommands == 0) {
        semaphore_wait(&fullBuffer);
        semaphore_wait(&binarySem);

        numberCommands--;
        return inputCommands[headQueue++];

        semaphore_post(&binarySem);
        semaphore_post(&emptyBuffer );
    } else {
        numberCommands--;
        return inputCommands[headQueue++];
    }
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

void* processInput(void* arg) {

    FILE* input = (FILE*) arg;
    char line[MAX_INPUT_SIZE];
    static int lineNumber = 0;

    while(fgets(line, sizeof(line)/sizeof(char), input)) {
        char token;
        char name[MAX_INPUT_SIZE];
        char newName[MAX_INPUT_SIZE];
        lineNumber++;

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
    insertCommand("q\n");
    return NULL;
}

void* applyCommands(){
    while(1){
        semaphore_wait(&commandsAccess);
        const char* command = removeCommand();

        char token;
        char name[MAX_INPUT_SIZE], newName[MAX_INPUT_SIZE];
        sscanf(command, "%c %s %s", &token, name, newName);
        printf("Command: %c %s %s\n", token, name, newName);

        int iNumber;
        switch (token) {
            case 'c':
                iNumber = obtainNewInumber(fs);
                semaphore_post(&commandsAccess);
                create(fs, name, iNumber);
                break;
            case 'l':
                semaphore_post(&commandsAccess);
                int searchResult = lookup(fs, name);
                if(!searchResult)
                    printf("%s not found\n", name);
                else
                    printf("%s found with inumber %d\n", name, searchResult);
                break;
            case 'd':
                semaphore_post(&commandsAccess);
                delete(fs, name);
                break;
            case 'r':
                semaphore_post(&commandsAccess);
                int oldSearchResult = lookup(fs, name);
                int newSearchResult = lookup(fs, newName);
                if (oldSearchResult && !newSearchResult) {
                    delete(fs, name);
                    create(fs, newName, oldSearchResult);
                }
                break;
            case 'q':
                semaphore_post(&commandsAccess);
                return NULL;
            default: { /* error */
                fprintf(stderr, "Error: command to apply\n");
                exit(EXIT_FAILURE);
            }
        }
    }
}


int main(int argc, char* argv[]) {
    TIMER start, end;

    parseArgs(argc, argv);

    fs = new_tecnicofs(numberBuckets);

    pthread_t* threads = (pthread_t*) malloc((numberThreads + 1) * sizeof(pthread_t));

    semaphore_init(&commandsAccess, 1);

    FILE* input = openFile(inputFile, "r");
    semaphore_init(&binarySem, 1);
    semaphore_init(&fullBuffer, 0);
    semaphore_init(&emptyBuffer, 1);

    TIMER_READ(start);

    thread_create(&threads[0], processInput, (void *)input);

    for (int i = 1; i < numberThreads + 1; i++) {
        thread_create(&threads[i], applyCommands, NULL);
    }
    for (int i = 0; i < numberThreads + 1; i++) {
        thread_join(threads[i]);
    }

    TIMER_READ(end);

    semaphore_destroy(&emptyBuffer);
    semaphore_destroy(&fullBuffer);
    semaphore_destroy(&binarySem);
    fclose(input);

    semaphore_destroy(&commandsAccess);

    printf("TecnicoFS completed in %.4f seconds.\n", TIMER_GET_DURATION(start, end));

    #if defined(MUTEX) || defined(RWLOCK)
        free(threads);
    #endif


    FILE* output = openFile(outputFile, "w");
    print_tecnicofs_tree(output, fs);
    fclose(output);

    free_tecnicofs(fs);
    exit(EXIT_SUCCESS);
}
