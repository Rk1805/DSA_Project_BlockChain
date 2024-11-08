// block.cpp
#include "block.h"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <openssl/sha.h>

// Constructor
Block::Block(int idx, const std::string& prevHash)
    : index(idx), previousHash(prevHash), nonce(0) {
    timestamp = getCurrentTimestamp();
    hash = calculateHash();
}

std::string Block::getHash() const { return hash; }
std::string Block::getPreviousHash() const { return previousHash; }

// Add a transaction to this block (only if there's space and the signature is valid)
bool Block::addTransaction(const Transaction& transaction, const std::string& publicKey) {
    if (transactions.size() >= MAX_TRANSACTIONS) {
        return false; // Block is full
    }
    if (!transaction.verifySignature(publicKey)) {
        std::cerr << "Transaction signature is invalid." << std::endl;
        return false; // Invalid signature
    }

    transactions.push_back(transaction);
    hash = calculateHash(); // Recalculate block hash when a new transaction is added
    return true;
}

// Display block details (for debugging and testing)
void Block::displayBlock() const {
    std::cout << "Block Index: " << index << std::endl;
    std::cout << "Previous Hash: " << previousHash << std::endl;
    std::cout << "Timestamp: " << timestamp << std::endl;
    std::cout << "Transactions:\n";
    for (const auto& tx : transactions) {
        std::cout << "  Sender: " << tx.getSender()
                  << ", Receiver: " << tx.getReceiver()
                  << ", Amount: " << tx.getAmount()
                  << ", Signature: " << tx.get
