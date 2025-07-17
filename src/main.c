#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "http_server.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        return 1;
    }

    char *ip = "";
    int port = atoi(argv[1]);
    int server_socket_fd = create_tcp_ipv4_socket();
    if(server_socket_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }
    struct sockaddr_in *server_address = create_ipv4_address(ip, port);

    int bindResult = bind(server_socket_fd, (struct sockaddr *)server_address, sizeof(*server_address));
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
    return 0;
}
