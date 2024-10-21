#include "Wallet.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor
Wallet::Wallet(const std::string& id) : id(id), balance(0.0f), privateKey(nullptr), publicKey(nullptr) {
    generateKeys();
}

// Destructor
Wallet::~Wallet() {
    RSA_free(privateKey);
    RSA_free(publicKey);
}

// Helper function to generate RSA key pairs
void Wallet::generateKeys() {
    const int keySize = 2048;
    BIGNUM* bn = BN_new();
    BN_set_word(bn, RSA_F4); // Common public exponent

    // Generate RSA keys
    privateKey = RSA_new();
    publicKey = RSA_new();
    if (!RSA_generate_key_ex(privateKey, keySize, bn, nullptr)) {
        std::cerr << "Error generating RSA keys: " << ERR_get_error() << std::endl;
    }
    BN_free(bn);

    // Extract the public key from the private key
    EVP_PKEY* evpKey = EVP_PKEY_new();
    EVP_PKEY_set1_RSA(evpKey, privateKey);
    RSA_up_ref(privateKey); // Increment reference count

    publicKey = EVP_PKEY_get1_RSA(evpKey);
    EVP_PKEY_free(evpKey);
}

// Create and sign a new transaction
Transaction Wallet::sendFunds(Wallet& receiver, float amount) {
    if (balance < amount) {
        std::cerr << "Insufficient funds!" << std::endl;
        return Transaction(); // Return an empty transaction
    }

    // Create a new transaction
    Transaction tx(id, receiver.id, amount);

    // Sign the transaction with the private key
    std::string txData = tx.getData();
    unsigned char signature[256];
    unsigned int sigLen;

    if (RSA_sign(NID_sha256, (const unsigned char*)txData.c_str(), txData.size(), 
                 signature, &sigLen, privateKey) != 1) {
        std::cerr << "Failed to sign transaction!" << std::endl;
    } else {
        tx.setSignature(std::string((char*)signature, sigLen));
    }

    return tx;
}

// Update balance based on a list of transactions
void Wallet::updateBalance(const std::vector<Transaction>& transactions) {
    for (const auto& tx : transactions) {
        if (tx.getSender() == id) {
            balance -= tx.getAmount();
        } else if (tx.getReceiver() == id) {
            balance += tx.getAmount();
        }
    }
}

// Print wallet data (ID, balance, public key)
void Wallet::printWalletData() const {
    std::cout << "Wallet ID: " << id << std::endl;
    std::cout << "Balance: " << std::fixed << std::setprecision(2) << balance << std::endl;
    std::cout << "Public Key: " << getPublicKeyString() << std::endl;
}

// Helper function to convert public key to string
std::string Wallet::getPublicKeyString() const {
    BIO* bio = BIO_new(BIO_s_mem());
    PEM_write_bio_RSAPublicKey(bio, publicKey);
    
    char* keyData;
    long keyLen = BIO_get_mem_data(bio, &keyData);
    
    std::string keyStr(keyData, keyLen);
    BIO_free(bio);
    return keyStr;
}
