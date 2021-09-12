#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_CLIENTS         100

static volatile int server_socket;
static volatile int session_ended;

static volatile int client_count;
static volatile int clients[MAX_CLIENTS];

static double start_time;

static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void establish_connection(char *port);
void close_connection(intptr_t client_id);
void *process_client(void *p);

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Usage: %s [port]\n", argv[0]);
        return 1;
    }

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    start_time = ts.tv_sec + ts.tv_nsec * 1e-9;

    establish_connection(argv[1]);

    pthread_t pthreads[MAX_CLIENTS];
    while(!session_ended) 
    {
        clients[client_count] = accept(server_socket, NULL, NULL);
        pthread_create(&pthreads[client_count], 0, process_client, 
                (void *)((intptr_t)client_count));
        client_count++;
    }

    return 0;
}

void establish_connection(char *port)
{
    server_socket = socket(AF_INET, SOCK_STREAM, 0); 
    if(server_socket == -1)
    {
        perror("socket");
        exit(1);
    }

    struct sockaddr_in socket_info;
    socket_info.sin_family = AF_INET;
    socket_info.sin_port = htons (atoi(port));
    socket_info.sin_addr.s_addr = htonl (INADDR_ANY);

    int bind_result = bind(server_socket, (struct sockaddr *) &socket_info, 
            sizeof(socket_info));
    if(bind_result != 0)
    {
        perror("bind");
        exit(1);
    }

    int listen_result = listen(server_socket, MAX_CLIENTS);
	if(listen_result != 0)
	{
		perror("listen");
		exit(1);
	}
}

void *process_client(void *p)
{
    pthread_detach(pthread_self());
    intptr_t client_id = (intptr_t)p;

    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    double connect_time = ts.tv_sec + ts.tv_nsec * 1e-9;

    char node_name[4096];
    memset(node_name, 0, 4096);
    recv(clients[client_id], node_name, 4096, 0);

    printf("%f - %s connected\n", connect_time, node_name);

    char log_buffer[65536];
    ssize_t bytes_logged = 0;

    char buffer[4096];
    while(1)
    {
        clock_gettime(CLOCK_REALTIME, &ts);
        double current_time = ts.tv_sec + ts.tv_nsec * 1e-9;

        memset(buffer, 0, 4096);
        ssize_t bytes_recd = recv(clients[client_id], buffer, 4096, 0);

        if(bytes_recd == 0)
        {
            break;
        }

        char *timestamp = strtok(buffer, " ");
        char *event = strtok(NULL, " ");

        printf("%s %s %s", timestamp, node_name, event);

        double timestamp_val;
        sscanf(timestamp, "%lf", &timestamp_val);

        bytes_logged += sprintf(log_buffer + bytes_logged, "%ld,%lf,%ld\n", 
                (long)(current_time - start_time), 
                current_time - timestamp_val, bytes_recd);

    }

    clock_gettime(CLOCK_REALTIME, &ts);
    double disconnect_time = ts.tv_sec + ts.tv_nsec * 1e-9;

    printf("%f - %s disconnected\n", disconnect_time, node_name);

    pthread_mutex_lock(&mutex);
    int log_fd = open("log", O_WRONLY | O_APPEND | O_CREAT, 
            S_IRWXU | S_IRWXG | S_IRWXO);
    write(log_fd, log_buffer, bytes_logged);
    close(log_fd);
    pthread_mutex_unlock(&mutex);

    close_connection(client_id);

    return NULL;
}

void close_connection(intptr_t client_id)
{
    pthread_mutex_lock(&mutex);
    shutdown(clients[client_id], SHUT_RDWR);
    close(clients[client_id]);
    clients[client_id] = -1;
    pthread_mutex_unlock(&mutex);
}
