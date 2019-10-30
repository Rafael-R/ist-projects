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
pthread_mutex_t commandsAccess;
tecnicofs* fs;

char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;


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

FILE* openFile(char* filename) {
    FILE *fptr;
    fptr = fopen(filename, "w");
    if (fptr == NULL) {
        fprintf(stderr, "Error: invalid file\n");
        exit(EXIT_FAILURE);
    }
    return fptr;
}

void* processInput() {
    char line[MAX_INPUT_SIZE];

    FILE* input = openFile(inputFile);

    while (fgets(line, sizeof(line)/sizeof(char), input)) {
        char token;
        char name[MAX_INPUT_SIZE], newName[MAX_INPUT_SIZE];

        int numTokens = sscanf(line, "%c %s %s", &token, name, newName);

        printf("Command: %c %s %s\n", token, name, newName);

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
                return NULL;
            case 'r':
                if(numTokens != 3)
                    errorParse();
                if(insertCommand(line))
                    break;
                return NULL;
            case '#':
                break;
            default: { /* error */
                errorParse();
            }
        }
    }
    fclose(input);
    return NULL;
}

void* applyCommands(){

    while(1){
        mutex_lock(&commandsAccess);

        if (numberCommands > 0) {

            const char* command = removeCommand();
            if (command == NULL){
                mutex_unlock(&commandsAccess);
                continue;
            }
            char token;
            char name[MAX_INPUT_SIZE], newName[MAX_INPUT_SIZE];
            sscanf(command, "%c %s %s", &token, name, newName);

            int iNumber;
            switch (token) {
                case 'c':
                    iNumber = obtainNewInumber(fs);
                    mutex_unlock(&commandsAccess);
                    create(fs, name, iNumber);
                    break;
                case 'l':
                    mutex_unlock(&commandsAccess);
                    int searchResult = lookup(fs, name);
                    if(!searchResult)
                        printf("%s not found\n", name);
                    else
                        printf("%s found with inumber %d\n", name, searchResult);
                    break;
                case 'd':
                    mutex_unlock(&commandsAccess);
                    delete(fs, name);
                    break;
                case 'r':
                    mutex_unlock(&commandsAccess);
                    int oldSearchResult = lookup(fs, name);
                    int newSearchResult = lookup(fs, newName);
                    if (oldSearchResult && !newSearchResult) {
                        delete(fs, name);
                        create(fs, newName, oldSearchResult);
                    }
                    break;
                default: { /* error */
                    mutex_unlock(&commandsAccess);
                    fprintf(stderr, "Error: command to apply\n");
                    exit(EXIT_FAILURE);
                }
            }
        } else {
            mutex_unlock(&commandsAccess);
            #if defined(MUTEX) || defined(RWLOCK)
                pthread_exit(NULL);
            #else
                return NULL;    // Return normal em caso de execucao sequencial
            #endif
        }
    }
}


int main(int argc, char* argv[]) {

    parseArgs(argc, argv);

    fs = new_tecnicofs(numberBuckets);

    #if defined(MUTEX) || defined(RWLOCK)
        pthread_t* threads = (pthread_t*) malloc((numberThreads) * sizeof(pthread_t));
    #endif

    processInput();

    TIMER start, end;

    mutex_init(&commandsAccess);

    TIMER_READ(start);

    #if defined(MUTEX) || defined(RWLOCK)
        for (int i = 0; i < numberThreads; i++) {
            thread_create(&threads[i], applyCommands);
        }
        for (int i = 0; i < numberThreads; i++) {
            thread_join(threads[i]);
        }
    #else
        applyCommands();    // Execucao sequencial
    #endif

    TIMER_READ(end);

    mutex_destroy(&commandsAccess);

    printf("TecnicoFS completed in %.4f seconds.\n", TIMER_GET_DURATION(start, end));

    FILE* output = openFile(outputFile);
    print_tecnicofs_tree(output, fs);
    fclose(output);

    free_tecnicofs(fs);
    exit(EXIT_SUCCESS);
}
