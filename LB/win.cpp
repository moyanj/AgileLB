// server.cpp
#include "server.h"

#ifdef IS_WIN

#include <winsock2.h>
Server::Server() : sock(INVALID_SOCKET) {
    // 初始化 Winsock
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        std::cerr << "WSAStartup failed: " << result << std::endl;
    }
}

bool Server::initServer(const char* ip, int port) {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "Failed to create socket" << std::endl;
        return false;
    }

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    if (bind(sock, (PSOCKADDR)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "Failed to bind socket" << std::endl;
        closesocket(sock);
        return false;
    }

    if (listen(sock, 5) == SOCKET_ERROR) {
        std::cerr << "Failed to listen on socket" << std::endl;
        closesocket(sock);
        return false;
    }

    return true;
}

int Server::acceptClient() {
    sockaddr_in client_addr;
    int client_len = sizeof(client_addr);
    SOCKET client_sock = accept(sock, (PSOCKADDR)&client_addr, &client_len);
    if (client_sock == INVALID_SOCKET) {
        std::cerr << "Failed to accept client" << std::endl;
        return -1;
    }
    return client_sock;
}

Server::~Server() {
    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        WSACleanup();
    }
}
#endif