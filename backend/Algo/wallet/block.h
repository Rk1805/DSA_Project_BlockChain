// block.h
#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "transaction.h"  // Include the Transaction header

class Block {
public:
    // Constructor
    Block(int index, const std::string& previousHash);

    // Getter for block hash
    std::string getHash() const;

    // Getter for previous block hash
    std::string getPreviousHash() const;

    // Add a transaction to this block (only if there’s space and the signature is valid)
    bool addTransaction(const Transaction& transaction, const std::string& publicKey);

    // Display the block details (for debugging and testing)
    void displayBlock() const;

private:
    int index;                       // Block index in the chain
    long long nonce;                 // Nonce for proof-of-work
    std::string previousHash;        // Hash of the previous block
    std::string hash;                // Hash of this block
    std::vector<Transaction> transactions;  // List of transactions in this block
    std::string timestamp;           // Timestamp when the block was created

    // Maximum transactions per block
    static const int MAX_TRANSACTIONS = 8;

    // Calculate the block's hash based on its contents
    std::string calculateHash() const;

    // Helper function to get the current timestamp as a string
    std::string getCurrentTimestamp() const;
};

#endif // BLOCK_H
