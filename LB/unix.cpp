#include "server.h"
#include "utils.h"
#include "config.h"
#onclide "app.h"

#ifdef IS_UNIX
#include "sockpp/tcp_acceptor.h"
#include "sockpp/version.h"

void run_echo(sockpp::tcp_socket sock) {
    char buf[512];
    sockpp::result<size_t> res;

    while ((res = sock.read(buf, sizeof(buf))) && res.value() > 0)
        sock.write_n(buf, res.value());

    cout << "Connection closed from " << sock.peer_address() << endl;
}

int run() {
    cout << "Sample TCP echo server for 'sockpp' " << sockpp::SOCKPP_VERSION << '\n' << endl;

    in_port_t port = Config["port"].as<int>();

    sockpp::initialize();

    error_code ec;
    sockpp::tcp_acceptor acc{port, 4, ec};

    if (ec) {
        cerr << "Error creating the acceptor: " << ec.message() << endl;
        return 1;
    }
    cout << "Awaiting connections on port " << port << "..." << endl;

    while (true) {
        sockpp::inet_address peer;

        // Accept a new client connection
        if (auto res = acc.accept(&peer); !res) {
            cerr << "Error accepting incoming connection: " << res.error_message() << endl;
        }
        else {
            cout << "Received a connection request from " << peer << endl;
            sockpp::tcp_socket sock = res.release();

            // Create a thread and transfer the new stream to it.
            thread thr(run_echo, std::move(sock));
            thr.detach();
        }
    }

    return 0;
}


#endif // IS_UNIX