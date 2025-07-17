#include <netinet/in.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#include "http_server.h"

struct accepted_socket* accepted_sockets[DEFAULT_BACKLOG];
int accepted_sockets_cnt = 0;

void start_http_server(char *ip, int port) {
    int server_socket_fd = create_tcp_ipv4_socket();
    if(server_socket_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in *server_address = create_ipv4_address(ip, port);

    int bindResult = bind(server_socket_fd, (struct sockaddr *)&server_address, sizeof(*server_address));
    if (bindResult < 0) {
        perror("Bind failed");
        close(server_socket_fd);
        exit(1);
    }

    int listenResult = listen(server_socket_fd, DEFAULT_BACKLOG);
    if(listenResult < 0) {
        perror("Listen failed");
        close(server_socket_fd);
        exit(1);
    }
    printf("Server listening on port %d\n", port);

    start_accepting_incoming_connections(server_socket_fd);

    shutdown(server_socket_fd, SHUT_RDWR);

    return;
}

int create_tcp_ipv4_socket() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in* create_ipv4_address(char *ip, int port) {
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));
    address->sin_family = AF_INET;
    address->sin_port = htons(port);
    if(strlen(ip) == 0) {
        address->sin_addr.s_addr = INADDR_ANY;
    } else {
        inet_pton(AF_INET, ip, &address->sin_addr.s_addr);
    }

    return address;
}

struct accepted_socket* accept_incoming_connection(int fd) {
    struct sockaddr_in client_address;
    socklen_t client_address_size = sizeof(struct sockaddr_in);
    int client_fd = accept(fd, (struct sockaddr*) &client_address, &client_address_size);

    struct accepted_socket* client_socket = malloc(sizeof(struct accepted_socket));
    client_socket->address = client_address;
    client_socket->socket_fd = client_fd;
    client_socket->is_successfull = client_fd > 0;

    if(!client_socket->is_successfull) {
        client_socket->error = client_fd;
    }

    return client_socket;
}

void start_accepting_incoming_connections(int fd) {
    while(1) {
        struct accepted_socket* client_socket = accept_incoming_connection(fd);
        accepted_sockets[accepted_sockets_cnt++] = client_socket;

        send_data_in_seperate_thread(client_socket);
    }
}

void send_data_in_seperate_thread(struct accepted_socket* client_socket) {
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, client_handler, client_socket);
}

void* client_handler(void* args) {
    struct accepted_socket* client_socket = (struct accepted_socket*)args;
    char buffer[4096];
    ssize_t bytes_read = recv(client_socket->socket_fd, buffer, sizeof(buffer) - 1, 0);

    if(bytes_read <= 0) {
        close(client_socket->socket_fd);
        free(client_socket);
        return NULL;
    }
    buffer[bytes_read] = '\0';

    printf("Client Request: %s\n", buffer);

    if(strncmp(buffer, "GET ", 4) == 0){
        char method[16], path[256];
        sscanf(buffer,"%s %s", method, path);

        if(strcmp(path, "/") == 0) {
            strcpy(path, "/index.html");
        }
        char local_file[512];
        snprintf(local_file, sizeof(local_file), "serverroot/%s", path);

        FILE *fp = fopen(local_file, "rb");
        if(!fp) {
            const char* not_found = "<h1>404 Not Found<h1>";
            char header[256];
            snprintf(header, sizeof(header),
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Content-Length: %zu\r\n"
                "Connection: close\r\n"
                "\r\n",
                strlen(not_found)
            );
            send(client_socket->socket_fd, header, strlen(header),0);
            send(client_socket->socket_fd, not_found, strlen(not_found),0);
        } else {
            fseek(fp, 0L, SEEK_END);
            long file_size = ftell(fp);
            rewind(fp);

            char* body = malloc(file_size);
            fread(body, 1, file_size, fp);
            fclose(fp);

            const char* content_type = "text/plain";
            if(strstr(path, ".html")) { content_type = "text/html"; }
            else if (strstr(path, ".css")) { content_type = "text/css"; }
            else if (strstr(path, ".js"))  { content_type = "application/javascript"; }
            else if (strstr(path, ".png")) { content_type = "image/png"; }
            else if (strstr(path, ".jpg")) { content_type = "image/jpeg"; }
            else if (strstr(path, ".svg")) { content_type = "image/svg+xml"; }
            else if (strstr(path, ".txt")) { content_type = "text/plain"; }

            char header[256];
            snprintf(header, sizeof(header),
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: %s\r\n"
                "Content-Length: %ld\r\n"
                "Connection: close\r\n"
                "\r\n",
                content_type, file_size);

            send(client_socket->socket_fd, header, strlen(header), 0);
            send(client_socket->socket_fd, body, file_size, 0);
            free(body);
        }
    }
    else {
        const char* body = "<html><body>501 Not Implemented</body></html>";
        char response[4096];
        snprintf(response, sizeof(response),
            "HTTP/1.1 501 Not Implemented\r\n"
            "Content-Type: text/html\r\n"
            "Content-Length: %zu\r\n"
            "Connection: close\r\n"
            "\r\n"
            "%s",
            strlen(body), body
        );
        send(client_socket->socket_fd, response, sizeof(response), 0);
    }

    printf("client %d disconnected.\n", client_socket->socket_fd);
    close(client_socket->socket_fd);
    free(client_socket);

    return NULL;
}
