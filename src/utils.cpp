#include "utils.h"
#include "bytetohex.h"
#include "nybbletohex.h"

#include <openssl/sha.h>

#include <cstdint>
#include <cstring>

inline char to_hex(int i) {
	return nybble_to_hex[i];
}

void to_hex_string(uint64_t num, char str[17]) {
	str[0] = nybble_to_hex[(num & 0xF000000000000000) >> 60];
	str[1] = nybble_to_hex[(num & 0x0F00000000000000) >> 56];
	str[2] = nybble_to_hex[(num & 0x00F0000000000000) >> 52];
	str[3] = nybble_to_hex[(num & 0x000F000000000000) >> 48];
	str[4] = nybble_to_hex[(num & 0x0000F00000000000) >> 44];
	str[5] = nybble_to_hex[(num & 0x00000F0000000000) >> 40];
	str[6] = nybble_to_hex[(num & 0x000000F000000000) >> 36];
	str[7] = nybble_to_hex[(num & 0x0000000F00000000) >> 32];
	str[8] = nybble_to_hex[(num & 0x00000000F0000000) >> 28];
	str[9] = nybble_to_hex[(num & 0x000000000F000000) >> 24];
	str[10] = nybble_to_hex[(num & 0x0000000000F00000) >> 20];
	str[11] = nybble_to_hex[(num & 0x00000000000F0000) >> 16];
	str[12] = nybble_to_hex[(num & 0x000000000000F000) >> 12];
	str[13] = nybble_to_hex[(num & 0x0000000000000F00) >> 8];
	str[14] = nybble_to_hex[(num & 0x00000000000000F0) >> 4];
	str[15] = nybble_to_hex[(num & 0x000000000000000F)];
	str[16] = '\0';
}

void sha256(const char *input, char *output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];

    SHA256_CTX ctx;
    SHA256_Init(&ctx);
    SHA256_Update(&ctx, input, strlen(input));
    SHA256_Final(hash, &ctx);

	output[0] = '\0';

	for (unsigned int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
		const char *byte_str = byte_to_hex_lookup[hash[i]];
		memcpy(&output[i * 2], byte_str, sizeof(char) * 2);
	}

	output[SHA256_DIGEST_LENGTH * 2] = '\0';
}