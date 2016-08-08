#include "krist.h"
#include "utils.h"

#include <openssl/sha.h>

#include <string>
#include <cstring>
#include <cstdlib>

void make_private_key(const char *password, char *pkey) {
    strcpy(pkey, "KRISTWALLET");
    strcat(pkey, password);
    sha256(pkey, pkey);
    strcat(pkey, "-000");
}

void make_v2_address(const char *pkey, char address[11]) {
    strcpy(address, "k");
	address[1] = '\0';

    char chars[9][3];
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    double_sha256(pkey, hash);

    int i;

    for (i = 0; i < 9; ++i) {
        strncpy(chars[i], hash, 2);
		chars[i][2] = '\0';
        double_sha256(hash, hash);
    }

    for (i = 0; i < 9;) {
        char pair[3];
        strncpy(pair, &hash[2 * i], 2);
        int index = strtol(pair, nullptr, 16) % 9;

        if (strcmp(chars[index], "") == 0) {
            char temp_hash[SHA256_DIGEST_LENGTH * 2 + 1];
            sha256(hash, temp_hash);
            strcpy(hash, temp_hash);
        } else {
            char *ch = chars[index];
            int nch = strtol(ch, nullptr, 16);
			char hex_string[2] = { hex_to_base36(nch), '\0' };
            strcat(address, hex_string);
            strcpy(chars[index], "");
            i++;
        }
    }

	address[10] = '\0';
}

void make_v1_address(const char *pkey, char *address) {
    char hash[SHA256_DIGEST_LENGTH * 2 + 1];
    sha256(pkey, hash);
    strncpy(address, hash, 10);
}