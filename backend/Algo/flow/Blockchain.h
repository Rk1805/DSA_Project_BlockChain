#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "Block.h"
#include "Transaction.h"
#include <vector>

class Blockchain {
private:
    std::vector<Block> chain;

public:
    Blockchain() {
        chain.emplace_back(std::vector<Transaction>(), "0"); // Genesis block
    }

    void addBlock(const std::vector<Transaction> &transactions) {
        std::string prevBlockHash = chain.back().calculateBlockHash();
        Block newBlock(transactions, prevBlockHash);
        chain.push_back(newBlock);
    }

    const std::vector<Block>& getChain() const {
        return chain;
    }
};

#endif
