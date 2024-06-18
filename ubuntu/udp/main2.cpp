#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main() { // 客户端

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        cerr << "socket error!" << endl;
        return -1;
    } else {
        cout << "socket success!" << endl;
    }

    sockaddr_in addr_server{};
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = ntohs(2345);
    addr_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    socklen_t len_server;

    char buf[128]{0};
    string buf_str;
    while (true) {
        cin >> buf_str;
        if (buf_str == "exit") {
            break;
        }
        ssize_t slen = sendto(sock, buf_str.c_str(), min(buf_str.size(), sizeof(buf) - 1), 0,
                              reinterpret_cast<sockaddr*>(&addr_server), sizeof(addr_server));
        if (slen <= 0) {
            cerr << "send error!" << endl;
            break;
        }
        memset(buf, 0, sizeof(buf));
        ssize_t rlen = recvfrom(sock, buf, sizeof(buf) - 1, 0, nullptr, nullptr);
        if (rlen <= 0) {
            cerr << "recv error!" << endl;
            break;
        } else {
            cout << "recv: " << buf << endl;
        }
        buf_str.clear();
    }

    shutdown(sock, SHUT_RDWR);

    return 0;
}