#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main() { // 服务器端

    int sock_server = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock_server == -1) {
        cerr << "socket error!" << endl;
        return -1;
    } else {
        cout << "socket success!" << endl;
    }

    sockaddr_in addr_server{};
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(2345);
    addr_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    int bind_server = bind(sock_server, reinterpret_cast<sockaddr*>(&addr_server), sizeof(addr_server));
    if (bind_server < 0) {
        cerr << "bind error!" << endl;
    } else {
        cout << "bind success!" << endl;
    }

    while (true) {
        sockaddr_in addr_client{};
        socklen_t len_client = sizeof(addr_client);
        char buf[128]{};
        ssize_t rlen = recvfrom(sock_server, buf, sizeof(buf) - 1, 0, reinterpret_cast<sockaddr*>(&addr_client), &len_client);
        if (rlen <= 0) {
            cerr << "recv error!" << endl;
            break;
        } else {
            cout << "recv: " << buf << endl;
        }
        ssize_t slen = sendto(sock_server, buf, rlen, 0, reinterpret_cast<sockaddr*>(&addr_client), len_client);
        if (slen <= 0) {
            cerr << "send error!" << endl;
            break;
        }
        memset(buf, 0, sizeof(buf));
    }

    shutdown(sock_server, SHUT_RDWR);
//    close(sock_server);

    return 0;
}
