#include "utils.h"

#include <openssl/sha.h>

#include <cstdint>
#include <sstream>

void to_hex_string(uint64_t num, std::string *str) {
    std::ostringstream ss;
    ss << std::hex << num;
    *str = ss.str();
}

void sha256(const std::string &input, std::string *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, input.c_str(), input.length());
    SHA256_Final(hash, &ctx);

    const size_t output_size = SHA256_DIGEST_LENGTH * 2 + 1;
    char out_buffer[output_size];

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        std::sprintf(out_buffer + (i * 2) * sizeof(char), "%02x", hash[i]);
    }

    *output = std::string(out_buffer);
}

char hex_to_base36(int input) {
    for (int i = 6; i <= 251; i += 7) {
        if (input <= i) {
            if (i <= 69) {
                return (char) ('0' + (i - 6) / 7);
            }

            return (char) ('a' + ((i - 76) / 7));
        }
    }

    return 'e';
}