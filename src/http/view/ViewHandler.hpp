#pragma once

#include "FileView.hpp"
#include "View.hpp"

#include <fstream>
#include <functional>
#include <memory>
#include <string>

namespace http
{
namespace view
{

class ViewHandler {
    public:
        static HttpResponse handle(const HttpMessage& request) {
            auto link = request.get_path();

            if (link_to_view.find(link) == link_to_view.end()) {
                FileView static_view(static_path + link);
                return invoke_method(request, static_view);
            }

            return invoke_method(request, *link_to_view[link]);
        }

        static void set_static_path(std::string path) {
            static_path = path;
        }

        static void add_file_view(std::string link, std::string path) {
            FileView* file_view = new FileView(static_path + "/" + path);
            link_to_view[link] = file_view;
        }

        static void clear_views() {
            for (auto& view : link_to_view) {
                delete view.second;
            }
        }

    private:
        ViewHandler() {}

        static HttpResponse invoke_method(const HttpMessage& request, View& view) {
            auto method = request.get_method();

            switch (method) {
                case HttpMethod::Get: return view.get_method(request);
                case HttpMethod::Head: return view.head_method(request);
                case HttpMethod::Put: return view.put_method(request);
                case HttpMethod::Post: return view.post_method(request);
                case HttpMethod::Delete: return view.delete_method(request);
            }

            return HttpResponse(HttpStatus::S_501_NOT_IMPLEMENTED);
        }

        static std::string static_path;
        static std::unordered_map<std::string, View*> link_to_view;
};

std::string ViewHandler::static_path(".");

std::unordered_map<std::string, View*> ViewHandler::link_to_view = {
};

}
}