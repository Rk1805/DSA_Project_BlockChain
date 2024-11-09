#include "Blockchain.h"
#include "Transaction.h"
#include <iostream>

int main() {
    int difficulty = 4; // Set the difficulty level
    Blockchain blockchain(difficulty);

    // Sample transactions
    std::vector<Transaction> transactions = {
        Transaction("Alice", "Bob", 10.0),
        Transaction("Bob", "Charlie", 5.0),
        Transaction("Alice", "Charlie", 2.5),
        Transaction("Charlie", "Bob", 3.0)
    };

    // Add a block with these transactions
    blockchain.addBlock(transactions);

    // New transactions for the next block
    std::vector<Transaction> newTransactions = {
        Transaction("Alice", "Dave", 7.5),
        Transaction("Dave", "Charlie", 4.5)
    };

    // Add another block
    blockchain.addBlock(newTransactions);

    // Print the blockchain
    for (const auto& block : blockchain.getChain()) {
        std::cout << "Previous Block Hash: " << block.previousBlockHash << "\n";
        std::cout << "Merkle Root: " << block.merkleRoot << "\n";
        std::cout << "Block Hash: " << block.hash << "\n";
        std::cout << "Nonce: " << block.nonce << "\n\n";
    }

    return 0;
}