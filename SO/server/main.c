#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>
#include "../tecnicofs-api-common.h"
#include "fs.h"

#define TIMER                           struct timeval
#define TIMER_READ(time)                if(gettimeofday(&(time), NULL)){perror("gettimeofday failed"); exit(EXIT_FAILURE);}
#define TIMER_GET_DURATION(start, stop) \
    (((double)(stop.tv_sec)  + (double)(stop.tv_usec / 1000000.0)) - \
     ((double)(start.tv_sec) + (double)(start.tv_usec / 1000000.0)))


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

static void displayUsage (const char* appName);
static void parseArgs (long argc, char* const argv[]);
void insertCommand(char* data);
char* removeCommand();
void errorParse(int lineNumber);
FILE* openFile(char* filename, const char* mode);
void* applyCommands();
void* connection_handler(void* arg);


int main(int argc, char* argv[]) {

    int server_socket, client_socket, status;
    struct sockaddr_un server_addr;

    parseArgs(argc, argv);

    fs = new_tecnicofs(numberBuckets);

    // Cria socket stream
    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check_status(server_socket, "server: can't open stream socket\n");

    // Elimina o nome, para o caso de já existir.
    unlink(socketName);

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socketName);

    status = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    check_status(status, "server: can't bind local address\n");

    status = listen(server_socket, MAX_CLIENTS);
    check_status(status, "server: can't listen\n");

    while (1) {
        client_socket = accept(server_socket, NULL, NULL);  // (struct sockaddr*) &client_addr, (socklen_t*) &c);
        check_status(client_socket, "server: can't accept socket\n");

        pthread_t client_thread;
        int *client = malloc(sizeof(int));
        *client = client_socket;

        thread_create(&client_thread, connection_handler, client);
        thread_join(client_thread);

    }

    return 0;
}



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
    numberBuckets = atoi(argv[3]);

    if (numberBuckets < 1) {
        fprintf(stderr, "Error: invalid number of buckets\n");
        exit(EXIT_FAILURE);
    }
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

void* connection_handler(void* arg) {
    int client_socket = *((int*) arg);
    int status;
    char recvline[MAX_INPUT_SIZE];

    while (1) {
        status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
        check_status(status, "server: receiving message\n");
        printf("CLIENT: %s", recvline);
        memset(recvline, 0, MAX_INPUT_SIZE);
    }
} 

/*
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

*/