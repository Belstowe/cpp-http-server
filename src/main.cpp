#include "ini_parser/INI.hpp"
#include "tcp_server/SelectTCPServer.hpp"
#include "http/HttpEnum.hpp"
#include "http/HttpMessage.hpp"
#include "http/HttpResponse.hpp"
#include "http/view/ViewHandler.hpp"

#include <iostream>
#include <stdexcept>

using tcp_server::SelectTCPServer;
using tcp_server::TCPMessageHandleReturn;

TCPMessageHandleReturn message_handle(socket_t, std::string&& message, std::string& response_string) {
    http::HttpMessage http_message;
    try {
        http_message.deserialize(message);
    } catch (const std::runtime_error& e) {
        response_string = http::HttpResponse(e.what(), "HTTP/1.0", "text/plain", http::HttpStatus::S_400_BAD_REQUEST);
        return TCPMessageHandleReturn::SuccessResponseClose;
    }
    // std::cout << http_message.get_method() << " " << http_message.get_path() << " " << http_message.get_version() << '\n';
    for (auto&& pair : http_message) {
        std::cout << pair.first << " : " << pair.second << '\n';
    }

    std::cout << '\n';

    auto response = http::view::ViewHandler::handle(http_message);
    response_string = response;

    switch (response.get_status()) {
        case http::HttpStatus::S_400_BAD_REQUEST:
        case http::HttpStatus::S_404_NOT_FOUND:
        case http::HttpStatus::S_405_METHOD_NOT_ALLOWED:
        case http::HttpStatus::S_500_INTERNAL_SERVER_ERROR:
        case http::HttpStatus::S_501_NOT_IMPLEMENTED:
        case http::HttpStatus::S_502_BAD_GATEWAY:
        case http::HttpStatus::S_503_SERVICE_UNAVAILABLE:
            return TCPMessageHandleReturn::SuccessResponseClose;
    }

    if (http_message.has_attribute("Connection")) {
        if (http_message["Connection"] == "keep-alive") {
            return TCPMessageHandleReturn::SuccessResponseKeepAlive;
        }
    }
    return TCPMessageHandleReturn::SuccessResponseClose;
}

void ini_settings_parse(const ini_parser::INI& settings, uint16_t& port) {
    if (settings["Global"].find("WWWRoot") != settings["Global"].end()) {
        http::view::ViewHandler::set_static_path(settings["Global"].at("WWWRoot"));
    }

    if (settings["Global"].find("Port") != settings["Global"].end()) {
        port = static_cast<uint16_t>(std::stoul(settings["Global"].at("Port")));
    }
}

int main() {
    uint16_t settings_port = 0;
    try {
        ini_parser::INI settings_ini("settings.ini");
        ini_settings_parse(settings_ini, settings_port);
    }
    catch (std::runtime_error& e) {
        std::clog << e.what() << std::endl;
    }

    try {
        SelectTCPServer tcp_server;
        auto port = tcp_server.listen_on(0U, settings_port);
        std::cout << "Listening on port " << port << "..." << std::endl;

        while (true) {
            tcp_server.handle_connections(message_handle);
        }
    }
    catch (std::runtime_error& e) {
        std::clog << e.what() << std::endl;
    }
}