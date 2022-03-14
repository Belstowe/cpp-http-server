#include "tcp_server/SelectTCPServer.hpp"

#include <iostream>
#include <stdexcept>

using tcp_server::SelectTCPServer;
using tcp_server::TCPMessageHandleReturn;

TCPMessageHandleReturn message_handle(socket_t socket, std::string&& message, std::string& response) {
    std::cout << "TCP_Server data:" << '\n';
    std::cout << "\t- Client socket: " << socket << '\n';
    std::cout << "\t- Message length: " << message.length() << '\n';
    std::cout << "\t- Message: " << message << '\n' << std::endl;

    response = "Thank you!";

    return TCPMessageHandleReturn::SuccessResponse;
}

int main() {
    try {
        SelectTCPServer tcp_server;
        auto port = tcp_server.listen_on();
        std::cout << "Listening on port " << port << "..." << std::endl;

        while (true) {
            tcp_server.handle_connections(message_handle);
        }
    }
    catch (std::runtime_error& e) {
        std::clog << e.what() << std::endl;
    }
}