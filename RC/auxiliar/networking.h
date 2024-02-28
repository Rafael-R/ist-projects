#ifndef NETWORKING_H
#define NETWORKING_H

#include <cstdio>
#include <string>
#include <sstream>
#include <cstring>
#include <fstream>
#include <iostream>
#include <netdb.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

class UDP
{
private:
    int fd;
    struct addrinfo hints, *res;
public:
    UDP() {
        // empty
    }

    void client(char* hostname, char* port) {
        struct timeval tv;
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            std::cerr << "[UDP] Error creating socket." << std::endl;
            exit(1);    // error
        }

        if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
            std::cerr << "[UDP] Error setting socket options." << std::endl;  
            exit(1);    // error
        }

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;

        if (getaddrinfo(hostname, port, &hints, &res) != 0) {
            std::cerr << "[UDP] Error getting adress." << std::endl;
            exit(1);    // error
        }
    }

    int server(char* port) {
        if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
            std::cerr << "[UDP] Error creating socket." << std::endl;
            exit(1);    // error
        }

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_DGRAM;
        hints.ai_flags = AI_PASSIVE;

        if (getaddrinfo(NULL, port, &hints, &res) != 0) {
            std::cerr << "[UDP] Error getting adress." << std::endl;
            exit(1);    // error
        }

        if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) {
            std::cerr << "[UDP] Error binding socket." << std::endl;
            exit(1);    // error
        }
    
        return fd;
    }

    std::string communicate(std::string request) {
        ssize_t n;
        char buffer[128];
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);
        
        if (sendto(fd, request.c_str(), request.length(),
                   0, res->ai_addr, res->ai_addrlen) == -1) {
            std::cerr << "[UDP] Error sending message." << std::endl;
            exit(1);    //error
        }


        if (recvfrom(fd, buffer, 128, 0, (struct sockaddr*) &addr, &addrlen) < 0) {
            std::cerr << "[UDP] Error receiving message. Trying again." 
            << std::endl;
            return communicate(request);
        }

        return std::string(buffer);
    }

    void close_connection() {
        freeaddrinfo(res);
        close(fd);
    }
};

class TCP
{
private:
    int fd;
    struct addrinfo hints, *res;
public:
    TCP() {
        // empty
    }

    void client(char* hostname, char* port) {
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            std::cerr << "[TCP] Error creating socket." << std::endl;
            exit(1);    // error
        }

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;

        if (getaddrinfo(hostname, port, &hints, &res) != 0) {
            std::cerr << "[TCP] Error getting adress." << std::endl;
            exit(1);    // error
        }
    }

    int server(char* port) {
        if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
            std::cerr << "[TCP] Error creating socket." << std::endl;
            exit(1);    // error
        }

        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_INET;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        if (getaddrinfo(NULL, port, &hints, &res) != 0) {
            std::cerr << "[TCP] Error getting adress." << std::endl;
            exit(1);    // error
        }

        if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) {
            std::cerr << "[TCP] Error binding socket." << std::endl;
            exit(1);    // error
        }

        if (listen(fd, 5) == -1) {
            std::cerr << "[TCP] Error listening." << std::endl;
            exit(1);    // error
        }

        return fd;
    }

    void communicate(std::string request, std::string nop) {
        ssize_t n;
        char buffer[128];
        struct sockaddr_in addr;
        socklen_t addrlen = sizeof(addr);

        if (connect(fd, res->ai_addr, res->ai_addrlen) == -1) {
            std::cerr << "[TCP] Error connecting to socket." << std::endl;
            exit(1);    // error
        }
        
        if (write(fd, request.c_str(), request.length()) == -1) {
            std::cerr << "[TCP] Error sending message." << std::endl;
            exit(1);    // error
        }

        memset(buffer, 0, 128);
        if (read(fd, buffer, 128) == -1) {
            std::cerr << "[TCP] Error receiving message." << std::endl;
            exit(1);    // error
        }

        std::string code, status;
        std::stringstream stream = std::stringstream(std::string(buffer));
        stream >> code >> status;

        if (status == nop) {
            std::cout << "Nothing to display." << std::endl;
            return;
        }

        memset(buffer, 0, 128);
        if ((n = read(fd, buffer, 128)) == -1) {
            std::cerr << "[TCP] Error receiving message." << std::endl;
            exit(1);    // error
        }

        char name[25];
        int size, remaining, read_data;
        sscanf(buffer, "%s %d %n", name, &size, &remaining);
        unsigned char *data = (unsigned char*) buffer + remaining;
        read_data = n - remaining;
        data[read_data] = '\0';

        std::string filename = "client/data/" + std::string(name);
        FILE *file = fopen(filename.c_str(), "wb");

        fwrite(data, 1, read_data, file);
        size -= read_data;

        while (size > 0) {  // FIXME remove last \n
            memset(buffer, 0, 128);
            n = read(fd, buffer, 128);
            if (n == 0) break;
            if (n < 0) {
                std::cerr << "[TCP] Error receiving message." << std::endl;
                exit(1);    // error
            }
            char *p = buffer;
            while(n > 0) {
                int w = fwrite(p, 1, n, file);
                if (w <= 0) {
                    std::cerr << "Error writing to file." << std::endl;
                    exit(1);    // error
                }
                n -= w;
                size -= w;
                p += w;
            }
        }

        fclose(file);

        freeaddrinfo(res);
        close(fd);
    }
};

#endif