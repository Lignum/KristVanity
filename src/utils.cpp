#include "utils.h"
#include "bytetohex.h"

#include <openssl/sha.h>

#include <cstdint>
#include <sstream>
#include <iomanip>

void to_hex_string(uint64_t num, std::string *str) {

}

void sha256(const std::string &input, std::string *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, input.c_str(), input.length());
    SHA256_Final(hash, &ctx);

	*output = "";

	for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
		output->append(byte_to_hex_lookup[hash[i]]);
	}
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