#ifndef BLOCK_H
#define BLOCK_H

#include "MerkleTree.h"
#include <vector>

class Block {
public:
    std::string previousBlockHash;
    std::string merkleRoot;
    std::vector<std::string> transactions;

    Block(const std::vector<Transaction> &transactions, const std::string &prevHash)
        : previousBlockHash(prevHash) {
        std::vector<std::string> txHashes;
        for (const auto &tx : transactions) {
            txHashes.push_back(sha256(tx.toString()));
        }
        MerkleTree merkleTree;
        merkleRoot = merkleTree.calculateMerkleRoot(txHashes);
    }

    std::string calculateBlockHash() const {
        return sha256(previousBlockHash + merkleRoot);
    }
};

#endif
