#pragma once

#include "FileView.hpp"
#include "View.hpp"

#include <string>
#include <fstream>

namespace http
{
namespace view
{

class ViewHandler {
    public:
        static HttpResponse handle(const HttpMessage& request) {
            auto link = request.get_path();

            if (link_to_view.find(link) == link_to_view.end()) {
                return HttpResponse(HttpStatus::S_404_NOT_FOUND);
            }

            return invoke_method(request, link_to_view[link]);
        }

    private:
        ViewHandler() {}

        static HttpResponse invoke_method(const HttpMessage& request, View& view) {
            auto method = request.get_method();

            switch (method) {
                case HttpMethod::Get: return view.get_method(request);
                case HttpMethod::Post: return view.post_method(request);
            }

            return HttpResponse(HttpStatus::S_400_BAD_REQUEST);
        }

        static std::unordered_map<std::string, View> link_to_view;
};

std::unordered_map<std::string, View> ViewHandler::link_to_view = {
    {"/", FileView("index.html")}
};

}
}