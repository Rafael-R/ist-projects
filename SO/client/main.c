#include "../tecnicofs-api-common.h"


int main(int argc, char* argv[]) {
    int client_socket, status;
    struct sockaddr_un server_addr;
    char name[] = "skrskr", socketName[] = "/tmp/";
    char command[MAX_INPUT_SIZE];

    // Cria socket stream
    client_socket = socket(AF_UNIX, SOCK_STREAM, 0);
    check_status(client_socket, "client: can't open stream socket");

    bzero((char *)&server_addr, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strcat(socketName, name);
    strcpy(server_addr.sun_path, socketName);

    status = connect(client_socket, (struct sockaddr *) &server_addr, sizeof(server_addr));
    check_status(status, "client: can't connect to server");

    while (1) {
        // Lê o input
        printf("Enter command: ");
        fgets(command, sizeof(command)/sizeof(char), stdin);

        status = send(client_socket, command, sizeof(command), 0);
        check_status(status, "client: can't send message");
    }

    /* Fecha o socket e termina */
    close(client_socket);

    return 0;
}