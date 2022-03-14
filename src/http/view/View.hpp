#pragma once

#include "http/HttpMessage.hpp"
#include "http/HttpResponse.hpp"

namespace http
{
namespace view
{
class View {
    public:
        virtual ~View() {}
        virtual HttpResponse get_method(const HttpMessage&);
        virtual HttpResponse head_method(const HttpMessage&);
        virtual HttpResponse put_method(const HttpMessage&);
        virtual HttpResponse post_method(const HttpMessage&);
        virtual HttpResponse delete_method(const HttpMessage&);
};
}
}