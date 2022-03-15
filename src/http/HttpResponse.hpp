#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <chrono>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "HttpEnum.hpp"

namespace http
{

class HttpResponse {
    private:
        std::unordered_map<std::string, std::string> parameters;
        HttpStatus status;
        std::string version;
        std::vector<char> content;

        std::string get_current_date() {
            std::stringstream date;
            auto now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
            date << std::put_time(std::gmtime(&now), "%a, %d %b %Y %X UTC");
            
            return date.str();
        }

    public:
        HttpResponse(HttpStatus status = HttpStatus::S_204_NO_CONTENT, const std::string& version = "HTTP/1.0")
        : status{status}, version{version}, content{}
        {
            parameters["Date"] = get_current_date();
            parameters["Server"] = http::server_name;
        }

        HttpResponse(const std::vector<char>& content, const std::string& version = "HTTP/1.0", const std::string& content_type = "text/plain", HttpStatus status = HttpStatus::S_200_OK)
        : status{status}, version{version}, content{content}
        {
            parameters["Date"] = get_current_date();
            parameters["Server"] = http::server_name;
            parameters["Content-type"] = content_type;
            parameters["Content-Length"] = std::to_string(content.size());
        }

        HttpResponse(const std::string& content, const std::string& version, const std::string& content_type = "text/plain", HttpStatus status = HttpStatus::S_200_OK)
        : HttpResponse{std::vector<char>{content.begin(), content.end()}, version, content_type, status}
        {}

        HttpResponse(const std::unordered_map<std::string, std::string>& attributes, HttpStatus status = HttpStatus::S_200_OK, const std::vector<char>& content = {}, const std::string& version = "HTTP/1.0")
        : status{status}, version{version}, content{content}
        {
            parameters["Date"] = get_current_date();
            parameters["Server"] = http::server_name;

            for (auto&& attribute : attributes) {
                parameters[attribute.first] = attribute.second;
            }
        }

        operator std::string() {
            std::string response_body(version + " " + status_to_string_map.at(status) + "\n");

            for (auto&& attribute : parameters) {
                response_body += attribute.first + ": " + attribute.second + "\n";
            }

            response_body += "\n";
            for (auto&& byte : content) {
                response_body += byte;
            }

            return response_body;
        }

        const HttpStatus& get_status() const {
            return status;
        }
};

}