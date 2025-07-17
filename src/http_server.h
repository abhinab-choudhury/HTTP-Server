#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <netinet/in.h>

#define DEFAULT_BACKLOG 10

struct accepted_socket {
    int socket_fd;
    struct sockaddr_in address;
    int error;
    int is_successfull;
};

int create_tcp_ipv4_socket();
struct sockaddr_in* create_ipv4_address(char*, int);
void start_accepting_incoming_connections(int);
struct accepted_socket* accept_incoming_connection(int);
void send_data_in_seperate_thread(struct accepted_socket*);
void* client_handler(void*);

#endif
