#pragma once

#include <string>

void to_hex_string(uint64_t num, std::string *str);

char hex_to_base36(int input);

void sha256(const std::string &input, std::string *output);

inline void double_sha256(const std::string &input, std::string *output) {
    sha256(input, output);
    sha256(*output, output);
}