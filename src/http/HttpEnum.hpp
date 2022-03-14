#pragma once

#include <iostream>
#include <unordered_map>

namespace http
{
    const char server_name[] = "CppHttp/0.4";

    enum class HttpMethod {
        Get,
        Post
    };

    const std::unordered_map<std::string, HttpMethod> string_to_method_map {
        {"GET", HttpMethod::Get},
        {"POST", HttpMethod::Post}
    };

    std::ostream& operator<<(std::ostream& os, const HttpMethod& http_method) {
        switch (http_method) {
            case HttpMethod::Get: os << "GET"; break;
            case HttpMethod::Post: os << "POST"; break;
            default: os << static_cast<int>(http_method);
        }
        return os;
    }

    enum class HttpStatus {
        S_200_OK,
        S_204_NO_CONTENT,
        S_304_NOT_MODIFIED,
        S_400_BAD_REQUEST,
        S_404_NOT_FOUND,
        S_500_INTERNAL_SERVER_ERROR,
        S_501_NOT_IMPLEMENTED,
        S_502_BAD_GATEWAY,
        S_503_SERVICE_UNAVAILABLE
    };

    const std::unordered_map<HttpStatus, std::string> status_to_string_map {
        {HttpStatus::S_200_OK, "200 OK"},
        {HttpStatus::S_204_NO_CONTENT, "204 No Content"},
        {HttpStatus::S_304_NOT_MODIFIED, "304 Not Modified"},
        {HttpStatus::S_400_BAD_REQUEST, "400 Bad Request"},
        {HttpStatus::S_404_NOT_FOUND, "404 Not Found"},
        {HttpStatus::S_500_INTERNAL_SERVER_ERROR, "500 Internal Server Error"},
        {HttpStatus::S_501_NOT_IMPLEMENTED, "501 Not Implemented"},
        {HttpStatus::S_502_BAD_GATEWAY, "502 Bad Gateway"},
        {HttpStatus::S_503_SERVICE_UNAVAILABLE, "503 Service Unavailable"}
    };

    std::ostream& operator<<(std::ostream& os, const HttpStatus& http_status) {
        os << status_to_string_map.at(http_status);
        return os;
    }
}