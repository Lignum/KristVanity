#pragma once

#include <string>

void make_private_key(const char *password, char *pkey);

void make_v2_address(const char *pkey, char address[11]);

void make_v1_address(const char *pkey, char address[11]);