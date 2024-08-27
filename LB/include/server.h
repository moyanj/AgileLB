#ifndef SERVER_H
#define SERVER_H
#include "utils.h"

#ifdef IS_WIN
#include <winsock2.h>
#endif

class Server {
private:
#ifdef IS_WIN
    SOCKET sock;
#else
    int sock; // socket 文件描述符
#endif
public:
    // 构造函数
    Server();
    // 初始化服务器，创建 socket 并绑定到指定的地址和端口
    bool initServer(const char* ip, int port);
    // 接受客户端连接
    int acceptClient();
    ~Server();
};
#endif