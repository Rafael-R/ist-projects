#include "../tecnicofs-api-common.h"


char* socketName;

static void parseArgs (long argc, char* const argv[]);
static void displayUsage (const char* appName);

int main(int argc, char* argv[]) {

    char sendline[MAX_INPUT_SIZE], recvline[MAX_INPUT_SIZE];

    parseArgs(argc, argv);

    tfsMount(socketName);

    while (1) {
        printf("Enter command: ");
        fgets(sendline, MAX_INPUT_SIZE, stdin);
        sendline[strlen(sendline) - 1] = '\0';
        status = send(client_socket, sendline, strlen(sendline), 0);
        check_status(status, "client: sending message\n");

        status = recv(client_socket, recvline, MAX_INPUT_SIZE, 0);
        check_status(status, "client: receiving message\n");
        printf("SERVER: %s\n", recvline);
        memset(recvline, 0, MAX_INPUT_SIZE);
    }

    tfsUnmount();

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
