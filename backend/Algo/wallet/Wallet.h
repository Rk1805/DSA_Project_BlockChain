#ifndef WALLET_H
#define WALLET_H

#include <string>
#include <vector>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include "Transaction.h"

class Wallet {
private:
    std::string id;
    float balance;
    RSA* privateKey;
    RSA* publicKey;

    void generateKeys(); // Private helper for key generation

public:
    Wallet(const std::string& id);
    ~Wallet(); // Destructor to free RSA keys

    Transaction sendFunds(Wallet& receiver, float amount);
    void updateBalance(const std::vector<Transaction>& transactions);
    void printWalletData() const;

    std::string getPublicKeyString() const; // Helper to convert public key to string
};

#endif // WALLET_H
