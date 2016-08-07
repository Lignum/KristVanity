#include "krist.h"
#include "utils.h"

#include <string>

void make_private_key(const std::string &password, std::string *pkey) {
    sha256("KRISTWALLET" + password, pkey);
    pkey->append("-000");
}

void make_v2_address(const std::string &pkey, std::string *address) {
    *address = "k";

    std::string chars[9];
    std::string hash;
    double_sha256(pkey, &hash);

    int i;

    for (i = 0; i < 9; ++i) {
        chars[i] = hash.substr(0, 2);
        double_sha256(hash, &hash);
    }

    for (i = 0; i < 9;) {
        std::string pair = hash.substr((size_t) (2 * i), 2);
        int index = std::stoi(pair, nullptr, 16) % 9;

        if (chars[index].empty()) {
            sha256(hash, &hash);
        } else {
            std::string *ch = &chars[index];
            int nch = std::stoi(*ch, nullptr, 16);
            *address += hex_to_base36(nch);
            chars[index] = "";
            i++;
        }
    }
}

void make_v1_address(const std::string &pkey, std::string *address) {
    sha256(pkey, address);
    *address = address->substr(0, 10);
}