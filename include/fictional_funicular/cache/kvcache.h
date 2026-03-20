//
// Created by jake on 3/20/26.
//

#ifndef LLM_INFERENCE_ENGINE_KVCACHE_H
#define LLM_INFERENCE_ENGINE_KVCACHE_H
#include "../../../third_party/onnx/onnxruntime_cxx_api.h"

#endif //LLM_INFERENCE_ENGINE_KVCACHE_H


namespace cache {
    struct KVCache {
        std::vector<Ort::Value> _keys;
        std::vector<Ort::Value> _values;

        void clear() {
            _keys.clear();
            _values.clear();
        }
    };
}