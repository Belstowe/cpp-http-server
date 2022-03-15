#include "View.hpp"

using http::HttpMessage;
using http::HttpResponse;
using http::view::View;

HttpResponse View::get_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_405_METHOD_NOT_ALLOWED);
}

HttpResponse View::head_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_405_METHOD_NOT_ALLOWED);
}

HttpResponse View::put_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_405_METHOD_NOT_ALLOWED);
}

HttpResponse View::post_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_405_METHOD_NOT_ALLOWED);
}

HttpResponse View::delete_method(const HttpMessage&) {
    return HttpResponse(HttpStatus::S_405_METHOD_NOT_ALLOWED);
}