#include "Blockchain.h"
#include "Transaction.h"
#include <iostream>
#include<string.h>

int main() {
    int difficulty = 4; // Set the difficulty level
    Blockchain blockchain(difficulty);

    float amount;
    std::string sender;
    std::string reciever;
    std::string previousBlockHash;
    std::cin>>sender;
    std::cin>>reciever;
    std::cin>>amount;
    std::cin>>previousBlockHash;
    // Sample transactions

    // New transactions for the next block
    std::vector<Transaction> newTransactions = {
        Transaction(sender,reciever,amount)
    };

    // Add another block
    blockchain.addBlock(newTransactions,previousBlockHash);

    // Print the blockchain
    const Block &block = blockchain.getChain().back();
        std::cout << block.previousBlockHash << "\n";
        std::cout << block.merkleRoot << "\n";
        std::cout << block.hash << "\n";
        std::cout << block.nonce << "\n\n";

    return 0;
}
