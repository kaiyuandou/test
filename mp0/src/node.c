#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>

int establish_connection(char *hostname, char *port);

int main(int argc, char *argv[])
{
    if(argc != 4)
    {
        printf("Usage: %s node_name address port\n", argv[0]);
        return 1;
    }

    int server_socket = establish_connection(argv[2], argv[3]); 
    send(server_socket, argv[1], strlen(argv[1]), 0);
    
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    while((nread = getline(&line, &len, stdin)) != -1) {
        send(server_socket, line, len, 0);
    }

    close(server_socket);
    free(line);

    return 0;
}

int establish_connection(char *hostname, char *port) 
{
    // Establish a socket file descriptor
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(server_socket == -1)
    {
        perror("socket");
        exit(1);
    }

    // Get address information
    struct addrinfo hints;
    struct addrinfo *result;
    memset(&hints, 0, sizeof(struct addrinfo)); 

    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    int addr_result = getaddrinfo(hostname, port, &hints, &result);
    if(addr_result != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(addr_result));
        exit(1);
    }

    int connect_result = connect(server_socket, result->ai_addr, 
            result->ai_addrlen);
    if(connect_result == -1)
    {
        perror("connect");
        exit(1);
    }

    free(result);

    return server_socket;
}
