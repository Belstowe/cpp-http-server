#include "get_mime_type.hpp"

#include "util/Utility.hpp"

#include <unordered_map>

const std::unordered_map<std::string, std::string> extension_to_mime {
    // 'text' types
    {"htm", "text/html"},
    {"html", "text/html"},
    {"css", "text/css"},
    {"csv", "text/csv"},
    {"md", "text/markdown"},
    {"php", "text/php"},
    {"xml", "text/xml"},
    // 'image' types
    {"gif", "image/gif"},
    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"svg", "image/svg+xml"},
    {"tiff", "image/tiff"},
    {"webp", "image/webp"},
    {"png", "image/png"},
    {"ico", "image/vnd.microsoft.icon"},
    // 'application' types
    {"json", "application/json"},
    {"js", "application/javascript"}
};

std::string mime::get_mime_type(const std::string& path) {
    auto path_tokens = util::tokenize(path, '/');
    auto& filename = *(path_tokens.rbegin());
    auto filename_tokens = util::tokenize(filename, '.');
    if (filename_tokens.size() == 1)
        return "text/plain";

    auto& file_extension = *(filename_tokens.rbegin());
    if (extension_to_mime.find(file_extension) == extension_to_mime.end())
        return "text/plain";
    
    return extension_to_mime.at(file_extension);
}