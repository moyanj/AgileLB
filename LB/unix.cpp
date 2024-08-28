#include "server.h"
#include "utils.h"
#include "config.h"
#include "app.h"

#ifdef IS_UNIX
#include <sockpp/tcp_acceptor.h>
#include <sockpp/version.h>
#include <thread>
#include <iostream>
#include <vector>
#include <system_error>

using namespace std;
void run_echo(sockpp::tcp_socket sock) {
    char buf[BUFFER_SIZE];
    try {
        sockpp::result<size_t> res;
        while ((res = sock.read(buf, sizeof(buf))) && res.value() > 0)
            sock.write_n(buf, res.value());
    } catch (const std::exception& e) {
        // 日志记录异常信息
        cerr << "Exception in run_echo: " << e.what() << endl;
    }

    cout << "Connection closed from " << sock.peer_address() << endl;
}

int run() {
    try {
        in_port_t port = Config["port"].as<int>();

        sockpp::initialize();

        sockpp::tcp_acceptor acc(port, 4);

        cout << "Awaiting connections on port " << port << "..." << endl;

        while (true) {
            sockpp::inet_address peer;
            auto res = acc.accept(&peer);

            if (!res) {
                cerr << "Error accepting incoming connection: " << res.error_message() << endl;
                continue;
            }

            cout << "Received a connection request from " << peer << endl;
            sockpp::tcp_socket sock = res.release();
            thread thr(run_echo, move(sock));
            thr.detach();
            
        }
    } catch (const std::exception& e) {
        // 日志记录异常信息
        std::cerr << "Exception in run: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
#endif // IS_UNIX

