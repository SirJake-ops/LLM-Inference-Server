//
// Created by jake on 1/13/26.
//

#include "../include/routes.h"
#include <iostream>

#include "onnxruntime_cxx_api.h"

void load_routes::Routes::start(const char *host, const int &port) {
    {//GET Requests
        get_hi();
        stop_server();
    }
    svr_.listen(host, port);
}

void load_routes::Routes::get_hi() {
    try {
        svr_.Get("/hi", [](const httplib::Request &req, httplib::Response &res) {
            res.set_content("Hello from the class", "text/plain");
        });
    } catch (...) {
        std::cerr << "Request not valid" << std::endl;
    }
}
void load_routes::Routes::stop_server() {
    try {
        svr_.Get("/stop", [&](const httplib::Request &req, httplib::Response &res) { svr_.stop(); });
    } catch (...) {
        std::cerr << "Server cannot stop." << std::endl;
    }
}
