//
// Created by jake on 1/13/26.
//

#include "../include/routes.h"
#include <iostream>

#include "../include/load_model_inference.h"
#include "onnxruntime_cxx_api.h"

std::vector<float> run_inference(const std::vector<std::int64_t> &input_ids) {
    model_inference::ModelInference model("../models/decoder_model.onnx");
    auto out_put = model.run_inference(input_ids);
    return out_put;
}

void load_routes::Routes::start(const char *host, const int &port) {
    { // GET Requests
        get_hi();
        run_model();
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

void load_routes::Routes::run_model() {
    try {
        svr_.Get("/run_model", [](const httplib::Request &req, httplib::Response &res) {
            const std::vector<std::int64_t> input_ids = {15496, 995};
            run_inference(input_ids);
        });
    } catch (const Ort::Exception e) {
        std::cerr << "Model failed to run: " << e.what() << std::endl;
    }
}

void load_routes::Routes::stop_server() {
    try {
        svr_.Get("/stop", [&](const httplib::Request &req, httplib::Response &res) { svr_.stop(); });
    } catch (...) {
        std::cerr << "Server cannot stop." << std::endl;
    }
}
