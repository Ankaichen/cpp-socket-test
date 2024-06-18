#include <iostream>
#include <WinSock2.h>
#include <algorithm>

using namespace std;

int main() { // 客户端

    // 启动网络库
    WORD version = MAKEWORD(2, 2);
    WSADATA dat;
    int nRes = WSAStartup(version, &dat);
    if (nRes != 0) {
        cout << "WSAStartup error: " << nRes << endl;
        return nRes;
    } else {
        cout << "WSAStartup success" << endl;
        cout << "veision: " << static_cast<int>(HIBYTE(dat.wVersion)) << ", " << static_cast<int>(LOBYTE(dat.wVersion)) << endl;
    }

    // 创建socket
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 连接服务器
    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(12345);
    // sin.sin_addr.S_un.S_addr = inet_addr("192.168.1.150");
//    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    sin.sin_addr.S_un.S_addr = inet_addr("192.168.88.129");
    int cRes = connect(sock, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
    if (cRes == SOCKET_ERROR) {
        cout << "connect error: " << cRes << endl;
    } else {
        cout << "connect success" << endl;
    }

    string buf;
    char refr[128] = {0};
    while (true) {
        cin.clear();
        cin >> buf;
        if (buf == "exit") {
            break;
        }
        int slen = send(sock, buf.c_str(), min<size_t>(buf.size(), 127), 0);
        buf.clear();
        if (slen == SOCKET_ERROR) {
            break;
        }
        int rlen = recv(sock, refr, sizeof(refr) - 1, 0);
        if (rlen == 0 || rlen == SOCKET_ERROR) {
            break;
        }
        cout << "recv: " << refr << endl;
        memset(refr, 0, 128);
    }

    closesocket(sock);
    WSACleanup();

    return 0;
}