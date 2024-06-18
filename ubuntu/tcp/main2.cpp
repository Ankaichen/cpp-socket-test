#include <iostream>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main() { // 客户端

    int sock = socket(AF_INET, SOCK_STREAM, 0);
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
    addr_server.sin_addr.s_addr = inet_addr("192.168.88.129");
    int connect_server = connect(sock, reinterpret_cast<sockaddr*>(&addr_server), sizeof(addr_server));
    if (connect_server < 0) {
        cerr << "connect error!";
    }

    char buf[128]{0};
    string buf_str;
    while (true) {
        int rlen = recv(sock, buf, sizeof(buf) - 1, 0);
        if (rlen <= 0) {
            cerr << "recv error!" << endl;
            break;
        } else {
            cout << "recv: " << buf << endl;
        }
        buf_str.clear();
        cin >> buf_str;
        if (buf_str == "exit") {
            break;
        }
        int slen = send(sock, buf_str.c_str(), min(buf_str.size(), sizeof(buf) - 1), 0);
        if (slen <= 0) {
            cerr << "send error!" << endl;
            break;
        }
        memset(buf, 0, sizeof(buf));
    }

    shutdown(sock, SHUT_RDWR);

    return 0;
}