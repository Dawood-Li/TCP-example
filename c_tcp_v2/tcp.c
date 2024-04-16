#include "tcp.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void default_on_open(int client_fd) {
    printf("Client connected: %d\n", client_fd);
}

void default_on_close(int client_fd) {
    printf("Client disconnected: %d\n", client_fd);
}

void default_on_msg(int client_fd, const char *msg, int len) {
    printf("Received message from client %d: %.*s\n", client_fd, len, msg);
}

void tcp_socket_on_open(TcpSocket *socket, void (*on_open)(int clientfd)) {
    socket->on_open = on_open;
}

void tcp_socket_on_close(TcpSocket *socket, void (*on_close)(int clientfd)) {
    socket->on_close = on_close;
}

void tcp_socket_on_msg(TcpSocket *socket, void (*on_msg)(int clientfd, const char * msg, int len)) {
    socket->on_msg = on_msg;
}

static inline TcpSocket * tcp_socket_init() {
    TcpSocket* sockfd = (TcpSocket*)malloc(sizeof(TcpSocket));
    if (sockfd == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    sockfd->on_open = NULL;
    sockfd->on_close = NULL;
    sockfd->on_msg = NULL;

    signal(SIGPIPE, SIG_IGN);

    if ((sockfd->sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

TcpSocket* tcp_server_init(const char *host, int port) {
    TcpSocket * sockfd = tcp_socket_init();

    int reuse = 1;
    if (setsockopt(sockfd->sockfd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    int keepalive = 1;
    if (setsockopt(sockfd->sockfd, SOL_SOCKET, SO_KEEPALIVE, &keepalive, sizeof(keepalive)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in socket_addr;
    memset(&socket_addr, 0, sizeof(socket_addr));
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &socket_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    if (bind(sockfd->sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }
    if (listen(sockfd->sockfd, 512) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void tcp_server_loop(TcpSocket *socket) {
    char recv_buf[65535];
    while (1) {
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int client = accept(socket->sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        if (socket->on_open) {
            socket->on_open(client);
        }

        while (1) {
            int recv_len = recv(client, recv_buf, sizeof(recv_buf), 0);
            if (recv_len <= 0) {
                socket->on_close(client);
                break;
            }
            socket->on_msg(client, recv_buf, recv_len);
        }

        close(client);
    }
}

TcpSocket * tcp_client_init(const char * host, int port) {
    TcpSocket * sockfd = tcp_socket_init();
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, host, &server_addr.sin_addr) <= 0) {
        perror("inet_pton");
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd->sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        exit(EXIT_FAILURE);
    }

    return sockfd;
}
