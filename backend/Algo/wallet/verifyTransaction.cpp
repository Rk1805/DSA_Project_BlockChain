#include <iostream>
#include <string>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/bio.h>
#include <openssl/sha.h>

// Function to verify the digital signature
bool verifySignature(const std::string& publicKey, const std::string& signature, const std::string& data) {
    // Initialize OpenSSL EVP_MD_CTX
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) {
        std::cerr << "Error creating EVP_MD_CTX" << std::endl;
        return false;
    }

    // Load public key from string (PEM format)
    BIO* bio = BIO_new_mem_buf(publicKey.c_str(), -1);
    EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL); // Use PEM_read_bio_PUBKEY instead
    BIO_free(bio);
    if (!pkey) {
        std::cerr << "Error loading RSA public key" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // Initialize verification context with SHA256
    if (EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pkey) != 1) {
        std::cerr << "Error initializing verification" << std::endl;
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        return false;
    }

    // Update with the data to be verified
    if (EVP_DigestVerifyUpdate(mdctx, data.c_str(), data.length()) != 1) {
        std::cerr << "Error updating verification with data" << std::endl;
        EVP_MD_CTX_free(mdctx);
        EVP_PKEY_free(pkey);
        return false;
    }

    // Verify the signature
    bool result = EVP_DigestVerifyFinal(mdctx, (const unsigned char*)signature.data(), signature.size()) == 1;

    // Clean up
    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);

    return result;
}

int main() {
    // Example inputs
    std::string publicKey;
    std::string signature;
    std::string sender;
    std::string to;
    float amount;

    // Input transaction details
    std::cin >> publicKey >> signature >> sender >> to;
    std::cin >> amount;

    // Concatenate transaction data to create a message for verification
    std::string data = sender + to + std::to_string(amount);

    // Verify the signature
    if (verifySignature(publicKey, signature, data)) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }

    return 0;
}
