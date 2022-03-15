#pragma once

#include "ITCP.hpp"

#include <cstdio>
#include <cstring>
#include <stdexcept>
#include <string>

namespace tcp_server
{
#define BUFLEN 4096

class SelectTCPServer : public ITCP
{
    private:
        sockaddr_in servAddr;
        socket_t sockMain;

        fd_set afds;
        socket_t nfds;

        int handle_method(socket_t sockClient, TCPMessageHandleReturn (*message_handle)(socket_t, std::string&&, std::string&))
        {
            char buf[BUFLEN + 1];
            ssize_t msgLength;

            memset(buf, 0, BUFLEN);
            if ((msgLength = recv(sockClient, buf, BUFLEN, 0)) < 0) {
                NIX(perror("recv()"));
                fprintf(stderr, "! Couldn't receive a message from socket " NIX("%d") WIN("%u") ".\n", sockClient);
                return -1;
            }
            else if (msgLength == 0)
                return 1;
            
            std::string response;
            auto code = message_handle(sockClient, std::move(std::string(buf, msgLength)), response);
            switch (code) {
                case TCPMessageHandleReturn::Fail:
                    fprintf(stderr, "! Couldn't handle a message from socket " NIX("%d") WIN("%u") ".\n", sockClient);
                    return -1;

                case TCPMessageHandleReturn::SuccessNoResponse:
                    break;
                
                case TCPMessageHandleReturn::SuccessResponseKeepAlive:
                    if (send(sockClient, response.c_str(), response.length(), 0) < 0) {
                        NIX(perror("send()"));
                        fprintf(stderr, "! Couldn't send a message to socket " NIX("%d") WIN("%u") ".\n", sockClient);
                        return -1;
                    }
                    return 0;
                
                case TCPMessageHandleReturn::SuccessResponseClose:
                    if (send(sockClient, response.c_str(), response.length(), 0) < 0) {
                        NIX(perror("send()"));
                        fprintf(stderr, "! Couldn't send a message to socket " NIX("%d") WIN("%u") ".\n", sockClient);
                        return -1;
                    }
                    break;
            }

            return 1;
        }

    public:
        SelectTCPServer()
        {
            #ifdef _WIN32
            struct WSAData w_data;

            if (WSAStartup(MAKEWORD(2, 2), &w_data) != 0) {
                throw std::runtime_error("Couldn't startup Windows Sockets 2.");
            }
            #endif

            sockMain = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (sockMain == WIN(INVALID_SOCKET) NIX(-1)) {
                NIX(perror("socket()"));
                throw std::runtime_error("Couldn't open TCP socket.");
            }
            memset(&servAddr, 0, sizeof(servAddr));
        }

        SelectTCPServer(uint32_t ip, uint16_t port)
        : SelectTCPServer()
        {
            listen_on(ip, port);
        }

        virtual ~SelectTCPServer() {}
        
        uint16_t listen_on(uint32_t ip = INADDR_ANY, uint16_t port = 0) override
        {
            servAddr.sin_family = AF_INET;
            servAddr.sin_addr.s_addr = htonl(ip);
            servAddr.sin_port = (port != 0) ? htons(port) : 0;

            if (bind(sockMain, (struct sockaddr *)&servAddr, sizeof(servAddr))) {
                NIX(perror("bind()"));
                throw std::runtime_error("Couldn't bind socket.");
            }

            socklen_t length = sizeof(servAddr);
            if (getsockname(sockMain, (struct sockaddr *)&servAddr, &length)) {
                NIX(perror("getsockname()"));
                throw std::runtime_error("Couldn't call 'getsockname'.");
            }

            if (listen(sockMain, 5) < 0) {
                NIX(perror("listen()"));
                throw std::runtime_error("Couldn't establish connections.");
            }

            nfds = getdtablesize();
            FD_ZERO(&afds);
            FD_SET(sockMain, &afds);

            return ntohs(servAddr.sin_port);
        }

        void handle_connections(TCPMessageHandleReturn (*message_handle)(socket_t, std::string&&, std::string&)) override
        {
            fd_set rfds;
            memcpy(&rfds, &afds, sizeof(rfds));
            if (select(nfds, &rfds, NULL, NULL, NULL) < 0) {
                NIX(perror("select()"));
                throw std::runtime_error("Couldn't select a socket.");
            }

            if (FD_ISSET(sockMain, &rfds)) {
                socket_t sockClient;
                if ((sockClient = accept(sockMain, NULL, NULL)) < 0) {
                    NIX(perror("accept()"));
                    throw std::runtime_error("Faulty client socket.");
                }
                FD_SET(sockClient, &afds);
            }
            
            for (socket_t fd = 0; fd < nfds; fd++) {
                if (fd != sockMain && FD_ISSET(fd, &rfds)) {
                    if (handle_method(fd, message_handle)) {
                        closesocket(fd);
                        FD_CLR(fd, &afds);
                    }
                }
            }
        }
};
}