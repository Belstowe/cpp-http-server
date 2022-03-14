#pragma once

#include <string>
#include <vector>

namespace http
{
std::vector<std::string> tokenize(std::string, char);
std::string&& trim(std::string&&);
}