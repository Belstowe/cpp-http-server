#pragma once

#include <string>
#include <vector>

namespace http
{
namespace util
{
std::vector<std::string> tokenize(std::string, char = '\n');
std::string&& trim(std::string&&);
}
}