/*
    一个tcp server示例
    将接收的数据原封发回
*/

// linux socket
#include <sys/socket.h>
#include <netinet/in.h> // for struct sockaddr_in
// #include <arpa/inet.h> // for ip地址转换

// linux kernel
#include <unistd.h> // for close
#include <signal.h>

// c++ standard library
#include <iostream>
#include <fstream>
#include <thread>
using namespace std;

// constant values
int PORT = 4396; // 端口号 

int main(int argc, char** argv) {
    
    // 若连接断开时仍发送 默认行为是无任何通知就结束进程
    signal(SIGPIPE, SIG_IGN);

    int server = socket(AF_INET, SOCK_STREAM, 0);

    if(argc == 2) { sscanf(argv[1], "%d", &PORT); }

    sockaddr_in server_addr = {
        .sin_family      = AF_INET,
        .sin_port        = htons(PORT),
        .sin_addr        = { .s_addr = INADDR_ANY }
    };

    if(bind(server, (sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        cerr<< "bind error." << endl;
        return -1;
    };

    /*
        listen第二个参数为 未连接队列大小 = 未完成三次握手 + 已完成三次握手
        最大队列大小查询指令如下 本机最大为512
        cat /proc/sys/net/ipv4/tcp_max_syn_backlog
        已完成握手队列满 则进入未完成握手队列 连接处于等待状态
        未完成握手队列满 则无法连接
        建议有独立的线程做连接工作
    */
    if(listen(server, 512) == -1) {
        cerr<< "listen error." << endl;
        return -2;
    };

    char recv_buf[65535];

    while(1) {

        sockaddr_in client_addr;
        unsigned client_addr_len = sizeof(client_addr);

        int client = accept(server, (sockaddr*)&client_addr, &client_addr_len);

        cout << "new connect from " << inet_ntoa(client_addr.sin_addr.s_addr) << ":" << ntohs(client_addr.sin_port) << endl;

        while(1) {
            
            int recv_len = recv(client, recv_buf, sizeof(recv_buf), 0);
            if (recv_len == 0) {
                cerr << "connect closed." << endl;
                break;
            }
            if (recv_len < 0) {
                cerr << "connect error." << endl;
                break;
            }
            cout << recv_buf << endl;
            send(client, recv_buf, recv_len, 0);
        }

        close(client);
    }
}
