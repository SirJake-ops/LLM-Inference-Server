# LLM Inference Engine

Small C++20 HTTP server that exposes text-generation routes and runs ONNX Runtime inference behind them.

## Requirements

- CMake 3.20+
- A C++20 compiler
- ONNX Runtime shared library for your platform
- A compatible ONNX model file at `models/model.onnx`

## Important: ONNX Runtime Is Not Included

This repository does not ship the ONNX Runtime `.so` file.

If you clone this project from GitHub, you must download ONNX Runtime yourself and provide the shared library locally before the project will build or run.

On Linux x86_64, the expected runtime library is typically one of these files from an ONNX Runtime release archive:

- `libonnxruntime.so.1.x.y`
- `libonnxruntime.so`

Example release archive:

- `onnxruntime-linux-x64-<version>.tgz`

After extracting the archive, you need the real shared library file from its `lib/` directory.

## Project Layout Expectations

This project currently expects:

- `models/libonnxruntime.so`
- `models/libonnxruntime.so.1`
- `models/model.onnx`

The `.so` files can be real files or symlinks to your extracted ONNX Runtime download.

Source layout:

```text
include/fictional_funicular/   public headers
src/                           application and library source
tests/                         test entrypoint and unit tests
examples/                      example or scratch integration code
third_party/                   vendored header-only/external headers
models/                        local runtime artifacts, not committed
```

Example:

```bash
ln -sfn /path/to/onnxruntime-linux-x64-1.24.4/lib/libonnxruntime.so.1.24.4 models/libonnxruntime.so
ln -sfn /path/to/onnxruntime-linux-x64-1.24.4/lib/libonnxruntime.so.1.24.4 models/libonnxruntime.so.1
```

## Build

From the project root:

```bash
cmake -S . -B build
cmake --build build
```

This produces the executable:

```bash
./build/LLM_Inference_Engine
```

## Run

Start the server from the project root:

```bash
./build/LLM_Inference_Engine
```

The server is configured to listen on:

- `127.0.0.1:1234`

Available routes:

- `GET /hi`
- `POST /generate`
- `GET /run_model` (legacy one-step debug route)
- `GET /stop`

Example:

```bash
curl http://127.0.0.1:1234/hi
curl -X POST "http://127.0.0.1:1234/generate?max_tokens=8" \
  --data "Hello, "
```

Example response:

```json
{
  "prompt": "Hello, ",
  "generated_text": "<tok:123><tok:456>",
  "response_text": "Hello, <tok:123><tok:456>",
  "prompt_token_ids": [72,101,108,108,111,44,32],
  "generated_token_ids": [123,456],
  "prompt_token_count": 7,
  "generated_token_count": 2,
  "cache_layers": 24,
  "cache_sequence_length": 9
}
```

## Request Flow

`POST /generate` is the main endpoint.

Request:

- The request body is the raw prompt text.
- `max_tokens` is an optional query parameter.
- If `max_tokens` is omitted, the server generates 16 new tokens.

Execution flow:

1. The route encodes the prompt into token ids.
2. The model cache is reset for that request.
3. The full prompt is sent once to prefill the model and initialize KV-cache state.
4. The server selects the best next token from the returned logits.
5. Each additional generation step sends only the latest token while reusing the cached keys and values from earlier steps.
6. The generated token ids are decoded and returned along with cache metadata.

## KV-Cache Behavior

- KV-cache state is held inside the shared `ModelInference` instance.
- The cache is reset at the start of every `/generate` request so prompts do not leak state into each other.
- During a single request, the first pass builds the cache from the prompt and later passes reuse it for token-by-token decoding.
- The response exposes `cache_layers` and `cache_sequence_length` so you can see how much cache state was retained after generation.

## Common Failure Modes

### Linker error for `-lonnxruntime`

Cause:

- ONNX Runtime is not installed or not wired into `models/`

Fix:

- Download ONNX Runtime
- Extract it
- Point `models/libonnxruntime.so` and `models/libonnxruntime.so.1` at the real library file

### Runtime error: `libonnxruntime.so.1` not found

Cause:

- The SONAME symlink is missing

Fix:

- Make sure both symlinks exist in `models/`:
  - `libonnxruntime.so`
  - `libonnxruntime.so.1`

### `/generate` fails

Cause:

- `models/model.onnx` is missing
- The model is incompatible with the inference code

Fix:

- Place the ONNX model file at `models/model.onnx`
- The shared library binary is intentionally not committed to this repository
