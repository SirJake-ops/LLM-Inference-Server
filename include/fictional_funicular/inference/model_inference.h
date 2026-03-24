//
// Created by jake on 1/14/26.
//

#ifndef LLM_INFERENCE_ENGINE_LOAD_MODEL_INFERENCE_H
#define LLM_INFERENCE_ENGINE_LOAD_MODEL_INFERENCE_H
#include "onnxruntime_cxx_api.h"
#include "fictional_funicular/cache/kvcache.h"
#include <filesystem>

namespace model_inference {
    class ModelInference {
    public:
        explicit ModelInference(const std::filesystem::path &path_to_model);

        std::size_t get_number_of_layers() const {
            return _layer_cache.size();
        }

        std::vector<float> run_inference(const std::vector<std::int64_t> &inputs, const int &number_of_layers);
        void reset_cache();
        std::size_t get_required_cache_layer_count() const;
        std::size_t get_cached_sequence_length() const;

    private:
        Ort::Env env_;
        Ort::Session session_;
        Ort::SessionOptions ort_session_options_;
        std::vector<cache::KVCache> _layer_cache;
        std::size_t required_cache_layer_count_{0};
    };
} // namespace model_inference

#endif // LLM_INFERENCE_ENGINE_LOAD_MODEL_INFERENCE_H
