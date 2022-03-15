#pragma once

#include <iostream>
#include <unordered_map>

namespace http
{
    const char server_name[] = "CppHttp/0.6";

    enum class HttpMethod {
        Get,
        Head,
        Post,
        Put,
        Delete
    };

    const std::unordered_map<std::string, HttpMethod> string_to_method_map {
        {"GET", HttpMethod::Get},
        {"HEAD", HttpMethod::Head},
        {"POST", HttpMethod::Post},
        {"PUT", HttpMethod::Put},
        {"DELETE", HttpMethod::Delete}
    };

    std::ostream& operator<<(std::ostream& os, const HttpMethod& http_method);

    enum class HttpStatus {
        S_200_OK,
        S_204_NO_CONTENT,
        S_304_NOT_MODIFIED,
        S_400_BAD_REQUEST,
        S_404_NOT_FOUND,
        S_405_METHOD_NOT_ALLOWED,
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
        {HttpStatus::S_405_METHOD_NOT_ALLOWED, "405 Method Not Allowed"},
        {HttpStatus::S_500_INTERNAL_SERVER_ERROR, "500 Internal Server Error"},
        {HttpStatus::S_501_NOT_IMPLEMENTED, "501 Not Implemented"},
        {HttpStatus::S_502_BAD_GATEWAY, "502 Bad Gateway"},
        {HttpStatus::S_503_SERVICE_UNAVAILABLE, "503 Service Unavailable"}
    };

    std::ostream& operator<<(std::ostream& os, const HttpStatus& http_status);
}