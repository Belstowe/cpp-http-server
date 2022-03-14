#pragma once

#include "Uninet.h"

#include <cinttypes>
#include <string>

namespace tcp_server
{
class ITCP
{
    public:
        virtual ~ITCP() {}
        virtual uint16_t listen_on(uint32_t ip, uint16_t port) = 0;
        virtual void handle_connections(int (*message_handle)(socket_t, std::string&&)) = 0;
};
}