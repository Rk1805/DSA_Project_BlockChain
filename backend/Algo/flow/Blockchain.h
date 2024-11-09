#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "Block.h"
#include "ProofOfWork.h"
#include <vector>

class Blockchain {
private:
    std::vector<Block> chain;
    int difficulty;

public:
    Blockchain(int diff = 4) : difficulty(diff) {
        chain.emplace_back(std::vector<Transaction>(), "0"); // Genesis block
    }

    void addBlock(const std::vector<Transaction>& transactions) {
        std::string prevBlockHash = chain.back().calculateBlockHash();
        Block newBlock(transactions, prevBlockHash);

        // Proof of Work
        ProofOfWork pow(difficulty);
        int nonce = 0;
        std::string minedHash = pow.mineBlock(nonce, newBlock.getData(), prevBlockHash);

        newBlock.setNonce(nonce);
        newBlock.setHash(minedHash);
        chain.push_back(newBlock);
    }

    const std::vector<Block>& getChain() const {
        return chain;
    }
};

#endif
