/*
    一个socket tcp client示例
    发送一段数据
    再接收一段数据

    1 测试连通性
    2 客户端最大发出2.8w个连接，而且超过1w连接后，响应速度肉眼可见。
    3 试试epoll server 会不会降低性能？
*/

// self
#include "./timer.h"

// linux socket
#include <sys/socket.h>
#include <netinet/in.h> // for struct sockaddr_in
#include <arpa/inet.h> // for ip地址转换

// linux kernel
#include <unistd.h> // for close

// c++ standard library
#include <iostream>
using namespace std;

// constant values
const char *ADDRESS = "localhost";
int PORT = 5000;

#define endl "\n"

int main(int argc, char** argv) {

    char send_buf[65536] = "hello"; 
    char recv_buf[65536] = ""; 

    // Timer t("1w times count");
    // for (int i = 0; i < 10000; i++) {

        int S = socket(AF_INET, SOCK_STREAM, 0);

        if(argc == 2) { sscanf(argv[1], "%d", &PORT); }
        if(argc == 3) { 
            ADDRESS = argv[1];
            sscanf(argv[2], "%d", &PORT);
        }

        sockaddr_in S_ad = {
            .sin_family      = AF_INET,
            .sin_port        = htons(PORT)
        };

        if(inet_pton(AF_INET, ADDRESS, &S_ad.sin_addr) < 0) {
            cerr<< "inet pton error." << endl;
            return -1;
        }

        cout << S << " " << ADDRESS << ":" << PORT << " connet success." << endl;

        if(connect(S, (sockaddr*)&S_ad, sizeof(S_ad)) != 0) {
            cerr << S << endl;
            perror("connect error.");
            return EXIT_FAILURE;
        }
            
        send(S, send_buf, 6, 0);
        
        int recv_len = recv(S, recv_buf, 65536, 0);

        cout << "send:" << send_buf << endl;
        cout << "recieve(" << recv_len << "):" << recv_buf << endl;

        close(S);
    // }

    return 0;
}
