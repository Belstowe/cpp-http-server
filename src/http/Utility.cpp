#include "Utility.hpp"

#include <sstream>

std::vector<std::string> http::tokenize(std::string message, char delimiter = '\n')
{
    std::vector<std::string> lines;

    std::stringstream line_spliter(message);

    std::string templine;

    while (std::getline(line_spliter, templine, delimiter)) {
        lines.emplace_back(std::move(templine));
    }

    return lines;
}

std::string&& http::trim(std::string&& str)
{
    str.erase(str.begin(), std::find_if(str.begin(), str.end(), [](const auto& c) {
        return !std::isspace(c);
    }));

    str.erase(std::find_if(str.rbegin(), str.rend(), [](const auto& c) {
        return !std::isspace(c);
    }).base(), str.end());

    return std::move(str);
}