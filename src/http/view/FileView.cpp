#include "FileView.hpp"

#include "mime/get_mime_type.hpp"

#include <fstream>
#include <string>
#include <vector>

using http::HttpMessage;
using http::HttpResponse;
using http::view::FileView;

HttpResponse FileView::get_method(const HttpMessage&)
{
    std::ifstream f(path.c_str(), std::ios::binary);
    if (!f.good()) {
        f.close();
        return HttpResponse(HttpStatus::S_404_NOT_FOUND);
    }

    std::vector<char> content(std::istreambuf_iterator<char>(f), {});
    f.close();

    return HttpResponse(content, "HTTP/1.0", mime::get_mime_type(path));
}

HttpResponse FileView::head_method(const HttpMessage&)
{
    std::ifstream f(path.c_str(), std::ios::ate | std::ios::binary);
    if (!f.good()) {
        f.close();
        return HttpResponse(HttpStatus::S_404_NOT_FOUND);
    }

    auto content_length = std::to_string(f.tellg());
    f.close();

    return HttpResponse(std::unordered_map<std::string, std::string>{
        {"Content-type", mime::get_mime_type(path)},
        {"Content-Length", content_length}
    });
}