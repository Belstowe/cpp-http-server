#pragma once

#include <string>
#include <vector>

namespace util
{
std::vector<std::string> tokenize(std::string, char = '\n');
std::string&& trim(std::string&&);
}