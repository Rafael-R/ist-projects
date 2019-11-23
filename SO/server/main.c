#include <pthread.h>
#include <signal.h>
#include <sys/time.h>
#include "../tecnicofs-api-common.h"
#include "fs.h"
#include "sync.h"

#define TIME struct timeval

typedef struct {
    struct sockaddr_un address;
    int socket;
    uid_t uid;
} client_t;


char* socketName;
char* outputFile;
int numberBuckets;

tecnicofs* fs;

volatile sig_atomic_t stop = 0;

static void parseArgs (long argc, char* const argv[]);
static void displayUsage (const char* appName);
void signal_handler(int sig);
void* connection_handler(void* arg);
char* applyCommands(uid_t client, char* command);
void readTime(TIME* time);
double getDuration(TIME start, TIME stop);


int main(int argc, char* argv[]) {

    int server_socket, client_socket, status;
    struct sockaddr_un server_addr, client_addr;
    pthread_t client_thread;
    static uid_t uid = 0;
    TIME start, end;

    parseArgs(argc, argv);

    fs = new_tecnicofs(numberBuckets);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check_status(server_socket, "server: can't open stream socket\n");

    unlink(socketName);

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socketName);

    status = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    check_status(status, "server: can't bind local address\n");

    status = listen(server_socket, MAX_CLIENTS);
    check_status(status, "server: can't listen\n");

    //signal(SIGINT, signal_handler);

    readTime(&start);

    while (!stop) {

        socklen_t clilen = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &clilen);
        check_status(client_socket, "server: can't accept socket\n");

        client_t* client = (client_t*) malloc(sizeof(client_t));
        client->address = client_addr;
        client->socket = client_socket;
        client->uid = uid++;
        
        thread_create(&client_thread, connection_handler, client);

    }

    puts("SERVER CLOSED\n");

    readTime(&end);

    printf("TecnicoFS completed in %.4f seconds.\n", getDuration(start, end));

    FILE* output = openFile(outputFile, "w");
    print_tecnicofs_tree(output, fs);
    fclose(output);

    free_tecnicofs(fs);
    exit(EXIT_SUCCESS);

    return 0;
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

static void displayUsage (const char* appName) {
    printf("Usage: ./%s <nomesocket> <outputfile> <numbuckets>\n", appName);
    exit(EXIT_FAILURE);
}

void signal_handler(int sig) {
    stop = 1;
}

void* connection_handler(void* arg) {
    client_t* client = (client_t*) arg;
    int status;
    char recvline[MAX_INPUT_SIZE];

    while (1) {
        status = recv(client->socket, recvline, MAX_INPUT_SIZE, 0);
        check_status(status, "server: receiving message\n");
        printf("CLIENT: %s\n", recvline);

        const char* sendline = applyCommands(client->uid, recvline);
        if (sendline != NULL) {
            status = send(client->socket, sendline, strlen(sendline), 0);
            check_status(status, "server: sending message\n");
        }

        memset(recvline, 0, MAX_INPUT_SIZE);
    }
}

char* applyCommands(uid_t client, char* command) {
    char* line = malloc(sizeof(char) * MAX_INPUT_SIZE);
    char token, arg1[MAX_INPUT_SIZE], arg2[MAX_INPUT_SIZE];
    int status;

    sscanf(command, "%c %s %s", &token, arg1, arg2);

    int iNumber;
    switch (token) {
        case 'c':
            iNumber = obtainNewINumber(fs);
            status = create_file(fs, client, arg1, iNumber, atoi(arg2));
            sprintf(line, "%d", status);
            break;
        case 'd':
            status = delete_file(fs, client, arg1);
            sprintf(line, "%d", status);
            break;
        case 'r':
            status = rename_file(fs, client, arg1, arg2);
            sprintf(line, "%d", status);
            break;
        case 'o':
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        case 'x':
            sprintf(line, "Command: %c %s", token, arg1);
            break;
        case 'l':
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        case 'w':
            sprintf(line, "Command: %c %s %s", token, arg1, arg2);
            break;
        default: {
            break;
        }
    }
    return line;
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
