// make sure to install these lib
//sudo apt update
//sudo apt install libcurl4-openssl-dev
//sudo apt update
//sudo apt install libjsoncpp-dev

//
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
    RSA* rsaKey = PEM_read_bio_RSAPublicKey(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (!rsaKey) {
        std::cerr << "Error loading RSA public key" << std::endl;
        EVP_MD_CTX_free(mdctx);
        return false;
    }

    // Convert RSA key to EVP_PKEY
    EVP_PKEY* pkey = EVP_PKEY_new();
    EVP_PKEY_assign_RSA(pkey, rsaKey);

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
    std::string publicKey = "-----BEGIN RSA PUBLIC KEY-----\n"
                            "MIIBigKCAYEA8r8u6yUWxJI1XSGkOCGI/pV7Y3zTkZH/EIlkHelvG39WsLMOFtuJ\n"
                            "q8T9Ovb3j05kBuzORIhuxqQqLaz1vsSZoLaplHQMrO8bKYz1HdXgogvJBZjMwRC9\n"
                            "odHAr1QSC1+qfYz+RptVQQOADkcw+LYoi65QoHGYi/slYhNni4AnlmGlnrcjHUKe\n"
                            "HtKsvNkDAWRKCk0bj9/yXHlkSO0fTp23rBYIaGGlBesPiA5vbtyc1YIoFLiBda2G\n"
                            "7saLCXMBP16kA2Jy0Ah4eiYPMqvePwc3RXEjJpbcxwbRtReLjRPHhh9mkW3etoMj\n"
                            "lZVJAMNO13GeuDkJ0P97OIOEq8v2fHqOq6SvEo5hwgmRhbHgIfry2t+VU1DuXjzC\n"
                            "rsnVHZzvDPGz+YiLlJZp4/PNByBJW90yUS6HOVKmGPYoO3TrxbN2JZMPF3zAEP7g\n"
                            "L7kVDz3dRieJ+PrNpMz/OlN88H+nSLtR8/yrIscsF28/cNnvJ2fB+NEt2CW4tBlA\n"
                            "Xgl/lPLmmCwJAgMBAAE=\n"
                            "-----END RSA PUBLIC KEY-----\n";

    // Example signature (replace with actual binary data)
    std::string signature = "Z\xE9\x14~\xB3e\x9Fu\xE7\x00|\x1A\x8B>\xC2\x05p\x9Bs\xC2)q\xC2\b\xC5\x89\xC4\x85:\x1E}";

    // Example transaction data
    std::string sender = "Rishi Kaneria";
    std::string to = "sahil";
    std::string amount = "100";

/*
 std::string url = "http://your-backend.com/api/transaction";

    // Fetch data from backend
    std::string jsonResponse = fetchDataFromBackend(url);

    // Parse JSON response
    Json::Reader reader;
    Json::Value obj;
    reader.parse(jsonResponse, obj);

    std::string publicKey = obj["publicKey"].asString();
    std::string signature = obj["signature"].asString();
    std::string sender = obj["sender"].asString();
    std::string to = obj["to"].asString();
    std::string amount = obj["amount"].asString();

*/
    // Concatenate transaction data to create a message for verification
    std::string data = sender + to + amount;

    // Verify the signature
    if (verifySignature(publicKey, signature, data)) {
        std::cout << "Transaction signature is valid!" << std::endl;
    } else {
        std::cout << "Transaction signature is invalid!" << std::endl;
    }

    return 0;
}
