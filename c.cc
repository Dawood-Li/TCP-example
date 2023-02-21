/*
    一个socket tcp client示例
    发送一段数据
    再接收一段数据
*/

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
// const char *ADDRESS = "w11";
int PORT = 4396;

int main(int argc, char** argv) {

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

    bind(S, (sockaddr*)&S_ad, sizeof(S_ad));

    cout << ADDRESS << ":" << PORT << " connetcing..." << endl;

    if(connect(S, (sockaddr*)&S_ad, sizeof(S_ad)) != 0) {
        cerr << "connect error." << endl;
        return -1;
    }

    char send_buf[65536] = "hello"; 
    char recv_buf[65536] = ""; 
        
    send(S, send_buf, 6, 0);
    
    int recv_len = recv(S, recv_buf, 65536, 0);

    cout << "send:" << send_buf << endl;
    cout << "recieve(" << recv_len << "):" << recv_buf << endl;

    close(S);
    return 0;
}
