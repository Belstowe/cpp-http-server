#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>

#include "HttpMethod.hpp"

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

            std::vector<std::string> lines;

            std::stringstream line_spliter(message);

            std::string templine;

            while (std::getline(line_spliter, templine)) {
                lines.emplace_back(std::move(templine));
            }

            std::string restful_line = lines.front();
            lines.erase(lines.begin());

            if (!(restful_line.empty()) && (restful_line.find("GET") == 0)) {
                std::stringstream token_spliter(restful_line);

                std::vector<std::string> tokens;

                while (std::getline(token_spliter, templine, ' ')) {
                    tokens.emplace_back(std::move(templine));
                }

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
                        parameters[line.substr(0, colon_pos)] = line.substr(colon_pos + 1);
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