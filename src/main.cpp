#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tcp_server/SelectTCPServer.hpp"

#include <iostream>

using tcp_server::SelectTCPServer;

int message_handle(socket_t socket, std::string&& message) {
    std::cout << "TCP_Server data:" << '\n';
    std::cout << "\t- Client socket: " << socket << '\n';
    std::cout << "\t- Message length: " << message.length() << '\n';
    std::cout << "\t- Message: " << message << '\n' << std::endl;

    return 0;
}

int main() {
    SelectTCPServer tcp_server;
    auto port = tcp_server.listen_on();
    std::cout << "Listening on port " << port << "..." << std::endl;

    while (true) {
        tcp_server.handle_connections(message_handle);
    }
}