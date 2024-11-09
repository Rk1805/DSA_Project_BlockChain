#include "Transaction.h"
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <stdexcept>

// Constructor
Transaction::Transaction(const std::string& sender, const std::string& receiver, float amount)
: sender(sender), receiver(receiver), amount(amount) {}

// Get transaction data (for signing)
std::string Transaction::getData() const {
return sender + receiver + std::to_string(amount);
}

std::string Transaction::getSender() const { return sender; }
std::string Transaction::getReceiver() const { return receiver; }
float Transaction::getAmount() const { return amount; }
std::string Transaction::getSignature() const { return signature; } 

void Transaction::setSignature(const std::string& sig) { signature = sig; }

// Method to sign the transaction
bool Transaction::signTransaction(const std::string& privateKey) {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) return false;

    // Load the private key
    BIO* bio = BIO_new_mem_buf(privateKey.c_str(), -1);
    EVP_PKEY* pkey = PEM_read_bio_PrivateKey(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (!pkey) return false;

    if (EVP_DigestSignInit(mdctx, NULL, EVP_sha256(), NULL, pkey) != 1) return false;

    std::string data = getData();
    if (EVP_DigestSignUpdate(mdctx, data.c_str(), data.length()) != 1) return false;

    // Finalize the signature
    size_t siglen;
    if (EVP_DigestSignFinal(mdctx, NULL, &siglen) != 1) return false;

    unsigned char* sig = (unsigned char*)malloc(siglen);
    if (!sig) return false;

    if (EVP_DigestSignFinal(mdctx, sig, &siglen) != 1) {
    free(sig);
    return false;
    }

    signature = std::string((char*)sig, siglen);
    free(sig);

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);

    return true;
}


// Method to verify the transaction's signature
bool Transaction::verifySignature(const std::string& publicKey) const {
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    if (!mdctx) return false;

    // Load the public key
    BIO* bio = BIO_new_mem_buf(publicKey.c_str(), -1);
    EVP_PKEY* pkey = PEM_read_bio_PUBKEY(bio, NULL, NULL, NULL);
    BIO_free(bio);
    if (!pkey) return false;

    if (EVP_DigestVerifyInit(mdctx, NULL, EVP_sha256(), NULL, pkey) != 1) return false;

    std::string data = getData();
    if (EVP_DigestVerifyUpdate(mdctx, data.c_str(), data.length()) != 1) return false;

    // Verify the signature
    bool result = EVP_DigestVerifyFinal(mdctx, (unsigned char*)signature.c_str(), signature.length()) == 1;

    EVP_MD_CTX_free(mdctx);
    EVP_PKEY_free(pkey);

    return result;
}

