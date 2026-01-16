# LLM Inference Engine

High-performance C++ inference server for large language models using ONNX Runtime.

## Features
- ✅ Fast C++ implementation
- ✅ ONNX Runtime integration
- ✅ REST API for inference
- ✅ Autoregressive text generation
- 🚧 Dynamic batching (planned)
- 🚧 Quantization support (planned)

## Quick Start
```bash
mkdir build && cd build
cmake ..
make
./LLM_Inference_Engine
```

## API Endpoints

### GET /run_model
Returns next token prediction
```
Response: next_token_id: 11
```

### Architecture
```
Client → HTTP Server → Model Inference → ONNX Runtime → Return Logits
```

## Performance
- Latency: X ms per token
- Throughput: Y tokens/sec

## Roadmap
- [ ] Dynamic batching
- [ ] Quantization (INT8/FP16)
- [ ] CUDA acceleration
- [ ] Streaming responses