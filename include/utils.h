#pragma once

#include "hextobase36.h"

#include <openssl/sha.h>

#include <string>

void to_hex_string(uint64_t num, char str[17]);

char hex_to_base36(int input);

void sha256(const char *input, char *output);

inline void double_sha256(const char *input, char output[SHA256_DIGEST_LENGTH * 2 + 1]) {
    char first_hash[SHA256_DIGEST_LENGTH * 2 + 1];
    sha256(input, first_hash);
    sha256(first_hash, output);
}

inline char hex_to_base36(int i) {
	return hex_to_base36_table[i];
}