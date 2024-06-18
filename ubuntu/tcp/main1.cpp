#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <cstring>

using namespace std;

int main() { // 服务器端

    int sock_server = socket(AF_INET, SOCK_STREAM, 0);
    if (sock_server == -1) {
        cerr << "socket error!" << endl;
        return -1;
    } else {
        cout << "socket success!" << endl;
    }

    sockaddr_in addr_server{};
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(2345);
//    addr_server.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_server.sin_addr.s_addr = inet_addr("192.168.88.129");
    int bind_server = bind(sock_server, reinterpret_cast<sockaddr*>(&addr_server), sizeof(addr_server));
    if (bind_server < 0) {
        cerr << "bind error!" << endl;
    } else {
        cout << "bind success!" << endl;
    }

    int listen_server = listen(sock_server, SOMAXCONN);
    if (listen_server < 0) {
        cerr << "listen error!" << endl;
    } else {
        cout << "listen success!" << endl;
    }

    while (true) {
        sockaddr_in addr_client{};
        socklen_t len_client = sizeof(addr_client);
        int sock_client = accept(sock_server, reinterpret_cast<sockaddr *>(&addr_client), &len_client);
        if (sock_client < 0) {
            cerr << "accept error!" << endl;
            break;
        } else {
            cout << "accept success!" << endl;
            cout << inet_ntoa(addr_client.sin_addr) << ":" << ntohs(addr_client.sin_port) << endl;
        }
        char buf[128]{"connect success!"};
        ssize_t slen = 17, rlen;
        while (true) {
            slen = send(sock_client, buf, slen, 0);
            if (slen <= 0) {
                cerr << "send error!" << endl;
                break;
            }
            memset(buf, 0, sizeof(buf));
            rlen = recv(sock_client, buf, sizeof(buf) - 1, 0);
            if (rlen <= 0) {
                cerr << "recv error!" << endl;
                break;
            } else {
                cout << "recv: " << buf << endl;
            }
            slen = rlen;
        }
        shutdown(sock_client, SHUT_RDWR);
    }




    shutdown(sock_server, SHUT_RDWR);
//    close(sock_server);

    return 0;
}
