#pragma once

#include "View.hpp"

#include <string>
#include <fstream>

namespace http
{
namespace view
{
class FileView : public View {
    private:
        std::string path;

    public:
        FileView(std::string path)
        : path{path}
        {}

        HttpResponse get_method(const HttpMessage&) override;
        HttpResponse head_method(const HttpMessage&) override;
};
}
}