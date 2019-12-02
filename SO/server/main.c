#define _GNU_SOURCE
#include <signal.h>
#include <errno.h>
#include <sys/time.h>
#include "fs.h"
#include "sync.h"

#define MAX_CLIENTS 100
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

volatile sig_atomic_t run = TRUE;

static void parseArgs (long argc, char* const argv[]);
static void displayUsage (const char* appName);
void checkStatus(int status, char* message);
static void signalHandler();
void* sessionHandler(void* arg);
void applyCommand(uid_t client, tempfile_t* files, char* message);
void readTime(TIME* time);
double getDuration(TIME start, TIME end);


int main(int argc, char* argv[]) {
    int server_socket, client_socket, status, clients = 0;
    struct sockaddr_un server_addr, client_addr;
    struct ucred client_data;
    socklen_t len;
    struct sigaction action;
    sigset_t signal_mask;
    TIME start, end;

    parseArgs(argc, argv);

    fs = newTecnicoFS(numberBuckets);   // numberBuckets e' ignorado

    sigemptyset (&signal_mask);
    status = pthread_sigmask(SIG_BLOCK, &signal_mask, NULL);
    if (status != 0) {
        fprintf(stderr, "Error: setting sigmask.");
        exit(EXIT_FAILURE);
    }
    action.sa_handler = signalHandler;
    sigemptyset(&action.sa_mask);
    action.sa_flags = 0; 
    sigaction(SIGINT, &action, NULL);

    server_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    checkStatus(server_socket, "Error: can't open stream socket.\n");

    unlink(socketName);

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socketName);

    status = bind(server_socket, (struct sockaddr*) &server_addr, sizeof(server_addr));
    checkStatus(status, "Error: binding local address.\n");

    status = listen(server_socket, 10);
    checkStatus(status, "Error: listening.\n");

    pthread_t* threads = (pthread_t*) malloc(sizeof(pthread_t) * MAX_CLIENTS);

    len = sizeof(client_addr);

    readTime(&start);

    while (run && clients < MAX_CLIENTS) {
        client_socket = accept(server_socket, (struct sockaddr*) &client_addr, &len);
        if (client_socket < 0) {
            if(errno == EINTR) {
                continue;
            } else {
                fprintf(stderr, "Error: can't accept socket.\n");
                exit(EXIT_FAILURE);
            }
        }

        len = sizeof(client_data);
        status = getsockopt(client_socket, SOL_SOCKET, SO_PEERCRED, &client_data, &len);
        checkStatus(status, "Error: reading client data.\n");

        client_t* client = (client_t*) malloc(sizeof(client_t));
        client->address = client_addr;
        client->socket = client_socket;
        client->uid = client_data.uid;

        thread_create(&threads[clients++], sessionHandler, client);
    }

    for (int i = 0; i < clients; i++) {
        thread_join(threads[i]);
    }

    readTime(&end);

    printf("TecnicoFS completed in %.4f seconds.\n", getDuration(start, end));

    FILE* output = fopen(outputFile, "w");
    if(!output){
        fprintf(stderr, "Error: opening file (%s).\n", outputFile);
        exit(EXIT_FAILURE);
    }
    printTecnicoFS(output, fs);
    fclose(output);

    freeTecnicoFS(fs);
    free(threads);
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
        fprintf(stderr, "Error: invalid number of buckets.\n");
        exit(EXIT_FAILURE);
    }
}

static void displayUsage (const char* appName) {
    printf("Usage: ./%s <nomesocket> <outputfile> <numbuckets>\n", appName);
    exit(EXIT_FAILURE);
}

void checkStatus(int status, char* message) {
    if(status < 0){
        fprintf(stderr, "%s", message);
        exit(EXIT_FAILURE);
    }
}

static void signalHandler() {
    printf("LOSING SERVER, WAITING FOR ALL CLIENTS TO DISCONNECT.\n");
    run = FALSE;
}

void* sessionHandler(void* arg) {
    client_t* client = (client_t*) arg;
    tempfile_t files[MAX_OPEN_FILES];
    char message[INPUT_SIZE];
    int status;

    for (int i = 0; i < MAX_OPEN_FILES; i++) {
        files[i].iNumber = -1;
        files[i].mode = NONE;
    }

    while (1) {
        memset(message, 0, INPUT_SIZE);

        status = recv(client->socket, message, INPUT_SIZE, 0);
        if (status < 0) {
            fprintf(stderr, "Error: receiving message.\n");
            exit(EXIT_FAILURE);
        } else if (status == 0) {
            break;
        }

        printf("CLIENT[%d]: %s\n", client->uid, message);
        applyCommand(client->uid, files, message);
        printf("SERVER->[%d]: %s\n", client->uid, message);

        status = send(client->socket, message, strlen(message), 0);
        checkStatus(status, "Error: sending message.\n");
    }

    close(client->socket);
    free(client);
    return NULL;
}

void applyCommand(uid_t client, tempfile_t files[], char* message) {
    char token, arg1[INPUT_SIZE], arg2[INPUT_SIZE], temp[INPUT_SIZE];
    int status;

    sscanf(message, "%c %s %s", &token, arg1, arg2);
    memset(message, 0, INPUT_SIZE);

    switch (token) {
        case 'c':
            status = createFile(fs, client, arg1, atoi(arg2));
            sprintf(message, "%d", status);
            break;
        case 'd':
            status = deleteFile(fs, client, arg1);
            sprintf(message, "%d", status);
            break;
        case 'r':
            status = renameFile(fs, client, arg1, arg2);
            sprintf(message, "%d", status);
            break;
        case 'o':
            status = openFile(fs, files, client, arg1, atoi(arg2));
            sprintf(message, "%d", status);
            break;
        case 'x':
            status = closeFile(fs, files, atoi(arg1));
            sprintf(message, "%d", status);
            break;
        case 'l':
            status = readFile(files, atoi(arg1), temp, atoi(arg2));
            sprintf(message, "%d %s", status, temp);
            memset(temp, 0, INPUT_SIZE);
            break;
        case 'w':
            status = writeFile(files, atoi(arg1), arg2);
            sprintf(message, "%d", status);
            break;
        default: {
            sprintf(message, "%d", TECNICOFS_ERROR_OTHER);
            break;
        }
    }
}

void readTime(TIME* time) {
    int status;
    status = gettimeofday(&(*time), NULL);
    checkStatus(status, "Error: reading time.\n");
}

double getDuration(TIME start, TIME end) {
    return (((double)(end.tv_sec)  + (double)(end.tv_usec / 1000000.0)) -
            ((double)(start.tv_sec) + (double)(start.tv_usec / 1000000.0)));
}
