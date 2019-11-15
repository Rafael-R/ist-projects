#include "../tecnicofs-api-common.h"



char* socketName;

static void displayUsage (const char* appName);
static void parseArgs (long argc, char* const argv[]);
void* processInput();


int main(int argc, char* argv[]) {

    int client_socket, status;
    struct sockaddr_un server_addr;
    char sendline[MAX_INPUT_SIZE];

    parseArgs(argc, argv);

    // Cria socket stream
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check_status(client_socket, "client: can't open stream socket\n");

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcpy(server_addr.sun_path, socketName);

    status = connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    check_status(status, "client: can't connect to server\n");

    
    while (1) {
        printf("Enter command: "); 
        fgets(sendline, MAX_INPUT_SIZE, stdin);
        status = send(client_socket, sendline, strlen(sendline), 0);
        check_status(status, "client: sending message\n");
    }

    /* Fecha o socket e termina */
    close(client_socket);

    return 0;
}



static void displayUsage (const char* appName) {
    printf("Usage: %s\n", appName);
    exit(EXIT_FAILURE);
}

static void parseArgs (long argc, char* const argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Invalid format:\n");
        displayUsage(argv[0]);
    }
    socketName = argv[1];
}

/*
void* processInput() {
    FILE* input = openFile(inputFile, "r");
    char line[MAX_INPUT_SIZE];
    int lineNumber = 0;

    while(fgets(line, sizeof(line)/sizeof(char), input)) {
        lineNumber++;

        char token;
        char name[MAX_INPUT_SIZE], newName[MAX_INPUT_SIZE];

        int numTokens = sscanf(line, "%c %s %s", &token, name, newName);

        // perform minimal validation
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
            default: { // error
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
*/