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
sem_t inputAccess;


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
    numberThreads = atoi(argv[3]);
    numberBuckets = atoi(argv[4]);

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
        semaphore_wait(&inputAccess);
    }
    strcpy(inputCommands[numberCommands++], data);
}

char* removeCommand() {
    if(numberCommands > 0){
        numberCommands--;
        semaphore_post(&inputAccess);
        return inputCommands[headQueue++];
    }
    return NULL;
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
    int lineNumber = 0;

    while (fgets(line, sizeof(line)/sizeof(char), input)) {
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
    return NULL;
}

void* applyCommands(){
    while(1){
        semaphore_wait(&commandsAccess);
        if (numberCommands > 0) {
            const char* command = removeCommand();
            if (command == NULL){
                semaphore_post(&commandsAccess);
                continue;
            }
            char token;
            char name[MAX_INPUT_SIZE], newName[MAX_INPUT_SIZE];
            sscanf(command, "%c %s %s", &token, name, newName);
            printf("command: %s", command);

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
                default: { /* error */
                    fprintf(stderr, "Error: command to apply\n");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            semaphore_post(&commandsAccess);
            return NULL;
        }
    }
}


int main(int argc, char* argv[]) {
    TIMER start, end;

    parseArgs(argc, argv);

    fs = new_tecnicofs(numberBuckets);

    #if defined(MUTEX) || defined(RWLOCK)
        pthread_t* threads = (pthread_t*) malloc((numberThreads + 1) * sizeof(pthread_t));
    #endif

    semaphore_init(&commandsAccess, 1);

    FILE* input = openFile(inputFile, "r");
    semaphore_init(&inputAccess, 1);

    TIMER_READ(start);

    #if defined(MUTEX) || defined(RWLOCK)
        thread_create(&threads[0], processInput, (void *)input);
        for (int i = 1; i < numberThreads; i++) {
            thread_create(&threads[i], applyCommands, NULL);
        }
        for (int i = 0; i < numberThreads; i++) {
            thread_join(threads[i]);
        }
    #else
        while (!feof(input)) {
            processInput((void *)input);
            applyCommands();    // Execucao sequencial
        }
    #endif

    TIMER_READ(end);

    semaphore_destroy(&inputAccess);
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
