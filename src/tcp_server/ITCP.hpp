#pragma once

#include "Uninet.h"

#include <cinttypes>
#include <string>

namespace tcp_server
{
enum class TCPMessageHandleReturn
{
    SuccessResponse,
    SuccessNoResponse,
    Fail
};

class ITCP
{
    public:
        virtual ~ITCP() {}
        virtual uint16_t listen_on(uint32_t ip, uint16_t port) = 0;
        virtual void handle_connections(TCPMessageHandleReturn (*message_handle)(socket_t, std::string&&, std::string&)) = 0;
};
}