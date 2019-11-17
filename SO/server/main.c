#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include "../tecnicofs-api-common.h"
#include "fs.h"
#include "sync.h"

#define TIME struct timeval

char* socketName;
char* outputFile;
int numberBuckets;

tecnicofs* fs;
char inputCommands[MAX_COMMANDS][MAX_INPUT_SIZE];
int numberCommands = 0;
int headQueue = 0;

volatile sig_atomic_t stop = 0;

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
char* applyCommands(char* command);
void* connection_handler(void* arg);
void signal_handler(int sig);
void readTime(TIME* time);
double getDuration(TIME start, TIME stop);


int main(int argc, char* argv[]) {

    //signal(SIGINT, signal_handler);

    int server_socket, client_socket, status;
    struct sockaddr_un server_addr, client_addr;
    pthread_t client_thread;
    TIME start, end;

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

    mutex_init(&commandLock);

    readTime(&start);

    while (!stop) {

        socklen_t clilen = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &clilen);
        check_status(client_socket, "server: can't accept socket\n");

        int *client = (int*) malloc(sizeof(int));
        *client = client_socket;
        thread_create(&client_thread, connection_handler, client);

    }

    puts("SERVER CLOSED\n");

    readTime(&end);

    mutex_destroy(&commandLock);

    printf("TecnicoFS completed in %.4f seconds.\n", getDuration(start, end));

    FILE* output = openFile(outputFile, "w");
    print_tecnicofs_tree(output, fs);
    fclose(output);

    free_tecnicofs(fs);
    exit(EXIT_SUCCESS);

    return 0;
}



static void displayUsage (const char* appName) {
    printf("Usage: ./%s <nomesocket> <outputfile> <numbuckets>\n", appName);
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

void errorParse(int lineNumber) {
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

char* applyCommands(char* command) {
    char* line = malloc(sizeof(char) * MAX_INPUT_SIZE);
    char token, arg1[MAX_INPUT_SIZE], arg2[MAX_INPUT_SIZE];

    mutex_lock(&commandLock);

    sscanf(command, "%c %s %s", &token, arg1, arg2);
    
    int iNumber;
    switch (token) {
        case 'c':
            iNumber = obtainNewInumber(fs);
            mutex_unlock(&commandLock);
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        case 'd':
            mutex_unlock(&commandLock);
            sprintf(line, "Command: %c %s", token, arg1);
            break;
        case 'r':
            mutex_unlock(&commandLock);
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        case 'o':
            mutex_unlock(&commandLock);
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        case 'x':
            mutex_unlock(&commandLock);
            sprintf(line, "Command: %c %s", token, arg1);
            break;
        case 'l':
            mutex_unlock(&commandLock);
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        case 'w':
            mutex_unlock(&commandLock);
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        default: {
            mutex_unlock(&commandLock);
        }
    }
    return line;
}

void* connection_handler(void* arg) {
    int client_socket = *((int*) arg);
    int status;
    char recvline[MAX_INPUT_SIZE];

    while (1) {
        status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
        check_status(status, "server: receiving message\n");
        printf("CLIENT: %s\n", recvline);

        const char* sendline = applyCommands(recvline);
        if (sendline != NULL) {
            status = send(client_socket, sendline, strlen(sendline), 0);
            check_status(status, "server: sending message\n");
        }

        memset(recvline, 0, MAX_INPUT_SIZE);
    }
}

void readTime(TIME* time) {
    int status;
    status = gettimeofday(&(*time), NULL);
    check_status(status, "gettimeofday failed\n");
}

double getDuration(TIME start, TIME stop) {
    return (((double)(stop.tv_sec)  + (double)(stop.tv_usec / 1000000.0)) -
            ((double)(start.tv_sec) + (double)(start.tv_usec / 1000000.0)));
}

void signal_handler(int sig) {
    stop = 1;
}