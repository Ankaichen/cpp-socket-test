#include <iostream>

#include <WinSock2.h>
#include <WS2tcpip.h>

using namespace std;

int main() { // 服务器端

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
    // 参数1 地址类型 AF_INET(IPv4) AF_INET6(IPv6)
    // 参数2 套节字类型 SOCK_STREAM(TCP) SOCK_DGRAM(UDP)
    // 参数3 协议类型 IPPROTO_TCP(TCP) IPPROTO_UDP(UDP)
    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // 绑定地址与端口
    sockaddr_in sin = {};
    sin.sin_family = AF_INET;
    sin.sin_port = htons(12345);
    // sin.sin_addr.S_un.S_addr = inet_addr("192.168.1.150");
    sin.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

    // 参数一 自己创建的socket
    // 参数二 一个sockaddr结构体的指针，存储着地址类型、IP地址、端口号信息
    // 参数三 参数二的类型大小 常用sizeof
    int bRes = bind(sock, reinterpret_cast<sockaddr*>(&sin), sizeof(sin));
    if (bRes == SOCKET_ERROR) {
        cout << "bind error: " << bRes << endl;
    } else {
        cout << "bind success" << endl;
    }

    // 监听网络端口
    // 参数一 服务器端socket
    // 参数二 挂起连接队列的最大长度
    int lRes = listen(sock, SOMAXCONN);
    if (lRes == SOCKET_ERROR) {
        cout << "listen error: " << lRes << endl;
    } else {
        cout << "listen success" << endl;
    }

    // 等待客户端连接
    sockaddr_in cSin = {};
    int cSinLen = sizeof(cSin);
    SOCKET cSock = accept(sock, reinterpret_cast<sockaddr*>(&cSin), &cSinLen);
    //  SOCKET cSock = accept(sock, nullptr, nullptr);
    if (cSock == INVALID_SOCKET) {
        cout << "accept error" << endl;
    } else {
        cout << "accept success" << endl;
        cout << inet_ntoa(cSin.sin_addr) << ":" << ntohs(cSin.sin_port) << endl;
    }

    // 接收消息
    // 参数一 客户端的socket
    // 参数二 接收消息的存储空间(协议规定网络最大传输单元为1500字节)
    // 参数三 想要读取的字节数，一般为参数二字节数减一
    // 参数四 一般为0：表示将协议缓冲区中的数据拷贝到Buf中，然后协议的缓冲区就删除这一部分已拷贝的数据(读出来就删除)
    //        MSG_PEEK：数据被复制到Buf中，但不会从协议的缓冲区中删除
    //        MSG_OOB：传输一段数据，在外带一个额外的特殊数据
    //        MSG_WAITALL：知道协议的缓冲区中数据的字节数满足参数三请求的字节数，才开始读取
    // 返回值 读出来的字节数大小，
    //        若客户端下线，返回0
    //        若执行失败，返回SOCKET_ERROR

    // 发送消息
    // 参数一 目标的socket
    // 参数二 给对方发送的数据
    // 参数三 字节个数
    // 参数四 一般为0，表示正常的发送
    //        MSG_OOB：意义同recv
    //        MSG_DONTROUTE：指定数据应不受路由限制
    // 返回值 成功则返回写入的字节数
    //        失败返回SOCKET_ERROR

    char buf[128] = {0};
    while (true) {
        int rlen = recv(cSock, buf, sizeof(buf) - 1, 0);
        cout << "receive data: " << buf << endl;
        if (rlen == 0 || rlen == SOCKET_ERROR) {
            break;
        }
        int slen = send(cSock, buf, rlen, 0);
        if (slen == SOCKET_ERROR) {
            break;
        }
        memset(buf, 0, 128);
    }

    

    closesocket(sock);
    closesocket(cSock);
    WSACleanup();

    return 0;
}
