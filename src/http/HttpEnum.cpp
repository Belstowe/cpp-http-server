#include "HttpEnum.hpp"

using http::HttpMethod;
using http::HttpStatus;
using http::status_to_string_map;

std::ostream& operator<<(std::ostream& os, const HttpStatus& http_status) {
    os << status_to_string_map.at(http_status);
    return os;
}

std::ostream& operator<<(std::ostream& os, const HttpMethod& http_method) {
    switch (http_method) {
        case HttpMethod::Get: os << "GET"; break;
        case HttpMethod::Head: os << "HEAD"; break;
        case HttpMethod::Post: os << "POST"; break;
        case HttpMethod::Put: os << "PUT"; break;
        case HttpMethod::Delete: os << "DELETE"; break;
        default: os << static_cast<int>(http_method);
    }
    return os;
}