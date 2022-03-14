#include "View.hpp"

using http::HttpMessage;
using http::HttpResponse;
using http::view::View;

HttpResponse View::get_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_400_BAD_REQUEST);
}

HttpResponse View::head_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_400_BAD_REQUEST);
}

HttpResponse View::put_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_400_BAD_REQUEST);
}

HttpResponse View::post_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_400_BAD_REQUEST);
}

HttpResponse View::delete_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_400_BAD_REQUEST);
}