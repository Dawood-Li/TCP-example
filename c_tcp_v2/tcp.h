#pragma once

#include <sys/socket.h>

typedef struct {
    int sockfd;
    void (*on_open)(int clientfd);
    void (*on_close)(int clientfd);
    void (*on_msg)(int clientfd, const char* msg, int len);
} TcpSocket;

void tcp_socket_on_open(TcpSocket *socket, void (*on_open)(int clientfd));

void tcp_socket_on_close(TcpSocket *socket, void (*on_close)(int clientfd));

void tcp_socket_on_msg(TcpSocket *socket, void (*on_msg)(int clientfd, const char * msg, int len));

TcpSocket* tcp_server_init(const char *host, int port);

void tcp_server_loop(TcpSocket *socket);

TcpSocket * tcp_client_init(const char * host, int port);
