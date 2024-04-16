#include "tcp.h"

#include <stdio.h>
#include <string.h>

int main() {
    TcpSocket * client = tcp_client_init("127.0.0.1", 8000);

    char buffer[1024];
    strcpy(buffer, "Hello, server!");
    send(client->sockfd, buffer, strlen(buffer), 0);
    
    memset(buffer, 0, sizeof(buffer));
    int n = recv(client->sockfd, buffer, sizeof(buffer), 0);
    if (n == 0) {
        printf("Server closed connection.\n");
        return 0;
    }
    if (n < 0) {
        perror("recv");
        return 0;
    }
    printf("Received message from server: %s\n", buffer);
    return 0;
}
