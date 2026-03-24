//
// Created by jake on 3/22/26.
//

#include "fictional_funicular/tokenizer/tokenizer.h"

#include <cstdint>
#include <string>
#include <vector>


std::vector<std::int64_t> token::Tokenizer::encode(const std::string& input) {
    std::vector<std::int64_t> tokens;
    tokens.reserve(input.size());

    for (const unsigned char byte : input) {
        tokens.push_back(static_cast<std::int64_t>(byte));
    }

    return tokens;
}

std::string token::Tokenizer::decode(const std::vector<std::int64_t>& tokens) {
    std::string output;
    output.reserve(tokens.size());

    for (const auto token_id : tokens) {
        if (token_id >= 0 && token_id <= 255) {
            output.push_back(static_cast<char>(token_id));
            continue;
        }

        output += "<tok:" + std::to_string(token_id) + ">";
    }
    return output;
}
