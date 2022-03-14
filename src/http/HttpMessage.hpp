#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "HttpEnum.hpp"
#include "Utility.hpp"

namespace http
{
class HttpMessage {
    private:
        std::unordered_map<std::string, std::string> parameters;
        HttpMethod method;
        std::string path;
        std::string version;
        bool valid;

    public:
        HttpMessage(const std::string& message) {
            valid = true;

            auto lines = http::tokenize(message, '\n');

            std::string restful_line = lines.front();
            lines.erase(lines.begin());

            if (!(restful_line.empty()) && (restful_line.find("GET") == 0)) {
                auto tokens = http::tokenize(restful_line, ' ');

                if ((tokens.size() == 3) && (tokens[0] == "GET") && (tokens[2].find("HTTP/") == 0)) {
                    method = HttpMethod::Get;
                    path = tokens[1];
                    version = tokens[2];
                }
                else {
                    valid = false;
                }

                for (auto&& line : lines) {
                    auto colon_pos = line.find(":");
                    if ((colon_pos != std::string::npos) && (colon_pos != 0)) {
                        parameters[http::trim(line.substr(0, colon_pos))] = http::trim(line.substr(colon_pos + 1));
                    }
                    else {
                        valid = false;
                    }
                }
            }
            else {
                valid = false;
            }
        }

        bool has_attribute(const std::string& key) const {
            return parameters.find(key) != parameters.end();
        }

        HttpMethod get_method() const {
            return method;
        }

        std::string get_path() const {
            return path;
        }

        std::string get_version() const {
            return version;
        }

        bool isValid() const {
            return valid;
        }

        std::string operator[](const std::string& key) const {
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