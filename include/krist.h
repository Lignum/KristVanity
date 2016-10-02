#pragma once

#include <string>

#include <openssl/sha.h>

#define KST_ADDRESS_LENGTH 10
#define KST_PRIVATEKEY_LENGTH SHA256_DIGEST_LENGTH * 2 + 1

void make_private_key(const char *password, char *pkey);

void make_v2_address(const char *pkey, char address[11]);

void make_v1_address(const char *pkey, char address[11]);