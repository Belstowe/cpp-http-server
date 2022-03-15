#pragma once

#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>

#include "HttpEnum.hpp"
#include "util/Utility.hpp"

namespace http
{
class HttpMessage {
    private:
        std::unordered_map<std::string, std::string> parameters;
        HttpMethod method;
        std::string path;
        std::string version;
        std::string content;

    public:
        HttpMessage()
        {}

        HttpMessage(const std::string& message) {
            deserialize(message);
        }

        void deserialize(const std::string& message) {
            auto lines = util::tokenize(message);

            for (auto& line : lines) {
                auto new_end = std::remove(line.begin(), line.end(), '\r');
                line.erase(new_end, line.end());
            }

            std::string restful_line = lines.front();
            lines.erase(lines.begin());

            if (restful_line.empty()) {
                throw std::runtime_error("Request is empty");
            }

            auto tokens = util::tokenize(restful_line, ' ');

            if (tokens.size() != 3) {
                throw std::runtime_error("Invalid HTTP request method format");
            }

            if (tokens[2].find("HTTP/") != 0) {
                throw std::runtime_error("Invalid HTTP request version");
            }

            if (string_to_method_map.find(tokens[0]) == string_to_method_map.end()) {
                throw std::runtime_error("Invalid/unsupported HTTP method");
            }

            method = string_to_method_map.at(tokens[0]);
            path = tokens[1];
            version = tokens[2];

            auto line_iterator = lines.begin();

            for (; !(*line_iterator).empty() && (line_iterator != lines.end()); line_iterator++) {
                auto&& header = *line_iterator;

                auto colon_pos = header.find(":");
                if ((colon_pos != std::string::npos) && (colon_pos != 0)) {
                    parameters[util::trim(header.substr(0, colon_pos))] = util::trim(header.substr(colon_pos + 1));
                }
                else {
                    throw std::runtime_error("Invalid header format");
                }
            }

            if (line_iterator != lines.end()) {
                line_iterator++;
            }

            for (; line_iterator != lines.end(); line_iterator++) {
                auto&& content_line = *line_iterator;

                if (!content_line.empty()) {
                    if ((method == HttpMethod::Get) || (method == HttpMethod::Head))
                        throw std::runtime_error(std::string() + tokens[0] + " method can't have response body!");
                }

                content += content_line;
            }
        }

        bool has_attribute(const std::string& key) const {
            return parameters.find(key) != parameters.end();
        }

        const HttpMethod& get_method() const {
            return method;
        }

        const std::string& get_path() const {
            return path;
        }

        const std::string& get_version() const {
            return version;
        }

        const std::string& get_content() const {
            return content;
        }

        const std::string& operator[](const std::string& key) const {
            return parameters.at(key);
        }

        std::unordered_map<std::string, std::string>::iterator begin() {
            return parameters.begin();
        }

        std::unordered_map<std::string, std::string>::iterator end() {
            return parameters.end();
        }

        std::unordered_map<std::string, std::string>::const_iterator begin() const {
            return parameters.cbegin();
        }

        std::unordered_map<std::string, std::string>::const_iterator end() const {
            return parameters.cend();
        }
};
}