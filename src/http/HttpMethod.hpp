#pragma once

#include <iostream>

namespace http
{
    enum class HttpMethod {
        Get,
        Post
    };

    std::ostream& operator<<(std::ostream& os, const HttpMethod& http_method) {
        switch (http_method) {
            case HttpMethod::Get:
                os << "GET";
                break;
            case HttpMethod::Post:
                os << "POST";
                break;
            default:
                os << static_cast<int>(http_method);
        }
        return os;
    }
}