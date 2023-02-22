#include "./tcp_server.hpp"

#include <iostream>
using namespace std;

int main(int argc, char** argv) {

    int port = 5000;
    
    if(argc == 2) { sscanf(argv[1], "%d", &port); }
    
    Tcp_Server server(port);

    server.on_new_connection = [] (const Tcp_Server::Msg& msg) {
        cout << "new connection fd: " << msg.fd
            << " from: " << msg.ip << ":" << msg.port << "\n";
    };

    server.on_message = [] (const Tcp_Server::Msg& msg) {
        cout << "message fd: " << msg.fd
            << " from: " << msg.ip << ":" << msg.port
            << " data[" << msg.data.size() << "]: " << msg.data << "\n";

        send(msg.fd, msg.data.c_str(), msg.data.size(), 0);
    };

    server.on_closed = [] (const Tcp_Server::Msg& msg) {
        cout << "connection closed fd: " << msg.fd
            << " from: " << msg.ip << ":" << msg.port << "\n";

    };

    server.on_error = [] (const Tcp_Server::Msg& msg) {
        cout << "connection error fd: " << msg.fd
            << " from: " << msg.ip << ":" << msg.port << "\n";
    };
}
