#ifndef SHA256_H
#define SHA256_H

#include <string>
#include <openssl/sha.h> // Ensure OpenSSL is installed

std::string sha256(const std::string &input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((unsigned char*)input.c_str(), input.size(), hash);

    std::string result;
    char buf[3];
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        snprintf(buf, sizeof(buf), "%02x", hash[i]);  // Use snprintf instead of sprintf
        result += buf;
    }
    return result;
}

#endif
