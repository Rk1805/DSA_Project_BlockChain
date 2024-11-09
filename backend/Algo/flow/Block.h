#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "Transaction.h"
#include "MerkleTree.h"
#include "ProofOfWork.h"

class Block {
public:
    std::string previousBlockHash;
    std::string merkleRoot;
    int nonce;
    std::string hash;
    std::vector<Transaction> transactions;

    Block(const std::vector<Transaction>& transactions, const std::string& prevHash)
        : previousBlockHash(prevHash), transactions(transactions), nonce(0), hash("") {
        MerkleTree merkleTree;
        merkleRoot = merkleTree.calculateMerkleRoot(getTransactionHashes());
    }

    std::string calculateBlockHash() const {
        return sha256(previousBlockHash + merkleRoot + std::to_string(nonce));
    }

    void setNonce(int newNonce) { nonce = newNonce; }
    void setHash(const std::string& newHash) { hash = newHash; }

    std::string getData() const {
        return previousBlockHash + merkleRoot;
    }

private:
    std::vector<std::string> getTransactionHashes() const {
        std::vector<std::string> hashes;
        for (const auto& transaction : transactions) {
            hashes.push_back(sha256(transaction.toString()));
        }
        return hashes;
    }
};

#endif
