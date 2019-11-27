#include "../tecnicofs-api-constants.h"


char* socketName;

static void parseArgs (long argc, char* const argv[]);
static void displayUsage (const char* appName);
void checkStatus(int status, char* message);

int main(int argc, char* argv[]) {

    int client_socket, status;
    struct sockaddr_un server_addr;
    char sendline[INPUT_SIZE], recvline[INPUT_SIZE];

    parseArgs(argc, argv);

    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    checkStatus(client_socket, "client: can't open stream socket\n");

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socketName);

    status = connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    checkStatus(status, "client: can't connect to server\n");


    while (1) {
        printf("Enter command: ");
        fgets(sendline, INPUT_SIZE, stdin);
        sendline[strlen(sendline) - 1] = '\0';
        status = send(client_socket, sendline, strlen(sendline), 0);
        checkStatus(status, "client: sending message\n");

        status = recv(client_socket, recvline, INPUT_SIZE, 0);
        checkStatus(status, "client: receiving message\n");
        printf("SERVER: %s\n", recvline);
        memset(recvline, 0, INPUT_SIZE);
    }

    close(client_socket);

    return 0;
}



static void parseArgs (long argc, char* const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid format:\n");
        displayUsage(argv[0]);
    }
    socketName = argv[1];
}

static void displayUsage (const char* appName) {
    printf("Usage: ./%s <nomesocket>\n", appName);
    exit(EXIT_FAILURE);
}

void checkStatus(int status, char* message) {
    if(status < 0){
        fprintf(stderr, "%s", message);
        exit(EXIT_FAILURE);
    }
}
