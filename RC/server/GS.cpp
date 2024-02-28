#include "game_protocol.h"

#define DEFAULT_PORT "58007"

using namespace std;


int main(int argc, char const *argv[])
{
    string filename = string(argv[1]);
    char port[6] = DEFAULT_PORT;
    bool verbose = false, skip = false;
    UDP udp;
    TCP tcp;
    int udp_fd, tcp_fd, maxfd, ready, connected, words = count_lines(filename);
    fd_set rset;
    ssize_t n;
    struct sockaddr_in addr;
    socklen_t addrlen;
    char buffer[128];
    pid_t childpid;
    request command;
    string arguments, reply;

    // Argument parsing
    if (argc == 5) {
        if (strcmp(argv[2], "-p") == 0 && strcmp(argv[4], "-v") == 0) {
            strcpy(port, argv[3]);
            verbose = true;
        } else
        if (strcmp(argv[2], "-v") == 0 && strcmp(argv[3], "-p") == 0) {
            strcpy(port, argv[4]);
            verbose = true;
        }
    } else if (argc == 3 && strcmp(argv[2], "-v") == 0) {
        verbose = true;
    } else if (argc == 4 && strcmp(argv[2], "-p") == 0) {
        strcpy(port, argv[3]);
    }

    // Create socket [UDP]
    udp_fd = udp.server(port);

    // Create socket [TCP]
    tcp_fd = tcp.server(port);

    FD_ZERO(&rset);

    maxfd = max(tcp_fd, udp_fd) + 1;

    while (true)
    {
        FD_SET(udp_fd, &rset);
        FD_SET(tcp_fd, &rset);

        ready = select(maxfd, &rset, NULL, NULL, NULL);

        if (FD_ISSET(udp_fd, &rset)) { 
            addrlen = sizeof(addr);

            memset(buffer, 0, 128);
            if (recvfrom(udp_fd, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen) == -1) {
                exit(1);    //error
            }

            if (verbose) {
            cout << inet_ntoa(addr.sin_addr) << ":" << 
                    addr.sin_port << " - " << buffer;
            }

            command = process_request(buffer, arguments);

            switch (command) {
                case SNG:
                    reply = start(arguments, filename, words);
                    break;
                case PLG:
                    reply = play(arguments);
                    break;
                case PWG:
                    reply = guess(arguments);
                    break;
                case QUT:
                    reply = quit(arguments);
                    break;
                case KILLGAME:
                    kill(arguments, 0);
                    skip = true;
                    break;
                case KILLPDIR:
                    kill(arguments, 1);
                    skip = true;
                    break;
                case ERR:
                    reply = "ERR\n";
                    break;
            }

            if (skip) {
                skip = false;
                continue;
            }

            if (sendto(udp_fd, reply.c_str(), reply.length(), 0, (struct sockaddr*) &addr, addrlen) == -1) {
                std::cerr << "[UDP] Error sending message." << std::endl;
                exit(1);    //error
            }
        }

        if (FD_ISSET(tcp_fd, &rset)) {
            addrlen = sizeof(addr);
            connected = accept(tcp_fd, (struct sockaddr*) &addr, &addrlen);
            if ((childpid = fork()) == 0) {
                close(tcp_fd);

                memset(buffer, 0, 128);
                if (read(connected, buffer, 128) == -1) {
                    std::cerr << "[TCP] Error receiving message." << std::endl;
                    exit(1);    // error
                }

                if (verbose) {
                cout << inet_ntoa(addr.sin_addr) << ":" << 
                        addr.sin_port << " - " << buffer;
                }

                string sendfile = "";
                string filepath;

                command = process_request(buffer, arguments);

                switch (command) {
                    case GSB:
                        reply = "RSB EMPTY\n";
                        break;
                    case GHL:
                        reply = hint(arguments, sendfile);
                        filepath = "server/data/" + sendfile;
                        break;
                    case STA:
                        reply = "RST NOK\n";
                        break;
                    case ERR:
                        reply = "ERR\n";
                        break;
                }

                if (write(connected, reply.c_str(), reply.length()) == -1) {
                    std::cerr << "[TCP] Error sending message." << std::endl;
                    exit(1);    // error
                }

                if (sendfile == "") {
                    close(connected);
                    exit(0);
                }

                ifstream file(filepath, std::ios::binary | std::ios::ate);
                streamsize size = file.tellg();
                string header = sendfile + " " + to_string(size) + " ";

                if (write(connected, header.c_str(), header.size()) == -1) {
                    std::cerr << "[TCP] Error sending message." << std::endl;
                    exit(1);    // error
                }

                while(!file.eof()) {
                    memset(buffer, 0, 128);
                    file.read(buffer, 128);
                    if (write(connected, buffer, 128) == -1) {
                        std::cerr << "[TCP] Error sending message." << std::endl;
                        exit(1);    // error
                    }
                }

                file.close();

                close(connected);
                exit(0);
            }
            close(connected);
        }
    }

    cout << "Closing..." << endl;
    udp.close_connection();

    return 0;
}
