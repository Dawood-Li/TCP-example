#include "tcp.h"

#include <stdio.h>

void on_open(int client_fd) {
    printf("Client connected: %d\n", client_fd);
}

void on_close(int client_fd) {
    printf("Client disconnected: %d\n", client_fd);
}

void on_msg(int client_fd, const char *msg, int len) {
    printf("Received message from client %d: %.*s\n", client_fd, len, msg);
    send(client_fd, msg, len, 0);
}

int main() {
    TcpSocket *server = tcp_server_init("0.0.0.0", 8000);
    tcp_socket_on_open(server, on_open);
    tcp_socket_on_close(server, on_close);
    tcp_socket_on_msg(server, on_msg);
    tcp_server_loop(server);
    return 0;
}
