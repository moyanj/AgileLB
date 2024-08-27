#include "server.h"
#include "client.h"
#include "utils.h"
#include "config.h"
#ifdef IS_UNIX
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <string>

using namespace std;

// 构造函数
Server::Server() {
    sock = -1; // 初始化为无效的 socket
}

    // 初始化服务器，创建 socket 并绑定到指定的地址和端口
bool Server::initServer(const char* ip, int port) {
    // 创建 socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Failed to create socket" << endl;
        return false;
    }

    // 设置服务器地址
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    // 绑定 socket 到地址
    if (bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Failed to bind socket" << endl;
        close(sock);
        return false;
    }

    // 开始监听
    if (listen(sock, WAITING_QUEUE_SIZE) < 0) { // 5 是等待队列的长度
        std::cerr << "Failed to listen on socket" << std::endl;
        close(sock);
        return false;
    }

    return true;
}

    // 接受客户端连接
int Server::acceptClient() {
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_len);
    if (client_sock < 0) {
        std::cerr << "Failed to accept client" << std::endl;
        return -1;
    }
    return client_sock;
}

    // 析构函数
Server::~Server() {
    if (sock >= 0) {
        close(sock);
    }
}

#endif // IS_UNIX