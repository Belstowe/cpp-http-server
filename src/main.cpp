#include "tcp_server/SelectTCPServer.hpp"
#include "http/HttpMessage.hpp"
#include "http/HttpResponse.hpp"

#include <iostream>
#include <stdexcept>

using tcp_server::SelectTCPServer;
using tcp_server::TCPMessageHandleReturn;

TCPMessageHandleReturn message_handle(socket_t, std::string&& message, std::string& response) {
    http::HttpMessage http_message(message);
    std::cout << "Received HTTP Message!" << '\n' << '\n';
    std::cout << http_message.get_method() << " " << http_message.get_path() << " " << http_message.get_version() << '\n';
    for (auto&& pair : http_message) {
        std::cout << pair.first << " : " << pair.second << '\n';
    }

    std::cout << '\n';

    response = http::HttpResponse("<html><body>Hello World!</body></html>", "HTTP/1.0", "text/html");

    if (http_message.has_attribute("Connection")) {
        if (http_message["Connection"] == "keep-alive") {
            return TCPMessageHandleReturn::SuccessResponseKeepAlive;
        }
    }
    return TCPMessageHandleReturn::SuccessResponseClose;
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