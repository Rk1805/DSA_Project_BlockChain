#include "Blockchain.h"
#include "Transaction.h"
#include <iostream>
#include<string.h>

using namespace std;

int main() {
    int difficulty = 4; // Set the difficulty level
    Blockchain blockchain(difficulty);

    float amount;
    string sender;
    string reciever;
    string previousBlockHash;
    cin>>sender;
    cin>>reciever;
    cin>>amount;
    cin>>previousBlockHash;
    // Sample transactions

    // New transactions for the next block
    vector<Transaction> newTransactions = {
        Transaction(sender,reciever,amount)
    };

    // Add another block
    blockchain.addBlock(newTransactions,previousBlockHash);

    // Print the blockchain
    const Block &block = blockchain.getChain().back();
        cout << block.previousBlockHash << "\n";
        cout << block.merkleRoot << "\n";
        cout << block.hash << "\n";
        cout << block.nonce << "\n\n";

    return 0;
}
