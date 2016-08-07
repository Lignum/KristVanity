#pragma once

#include <string>

void make_private_key(const std::string &password, std::string *pkey);

void make_v2_address(const std::string &pkey, std::string *address);

void make_v1_address(const std::string &pkey, std::string *address);