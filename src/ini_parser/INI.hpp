#pragma once

#include <fstream>
#include <regex>
#include <string>
#include <vector>
#include <unordered_map>

#include "util/Utility.hpp"

namespace ini_parser
{

class INI
{
    private:
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> ini_content;

    public:
        INI(std::string path)
        {
            std::string section = "Global";
            const std::regex section_rule(R"(^\[[A-Za-z]+(?=\]$))", std::regex_constants::icase);

            std::ifstream f(path.c_str());
            if (!f.good()) {
                f.close();
                throw std::runtime_error(std::string() + "Couldn't access INI file '" + path + "'!");
            }

            std::string line;
            std::smatch m;
            while (std::getline(f, line)) {
                if (line.empty())
                    continue;
                
                if (std::regex_search(line, m, section_rule)) {
                    section = std::string(m[0]).substr(1);
                    continue;
                }

                auto equal_pos = line.find("=");
                if ((equal_pos != std::string::npos) && (equal_pos != 0)) {
                    ini_content[section][util::trim(line.substr(0, equal_pos))] = util::trim(line.substr(equal_pos + 1));
                }
                else {
                    f.close();
                    throw std::runtime_error(std::string() + "'" + path + "': invalid format of line '" + line + "'!");
                }
            }

            f.close();
        }

        std::unordered_map<std::string, std::string>& operator[](const std::string& key)
        {
            return ini_content[key];
        }

        const std::unordered_map<std::string, std::string>& operator[](const std::string& key) const
        {
            return ini_content.at(key);
        }
};

}