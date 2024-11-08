#ifndef MERKLETREE_H
#define MERKLETREE_H

#include "SHA256.h"
#include <vector>

class MerkleTree {
public:
    std::string calculateMerkleRoot(const std::vector<std::string> &transactionHashes) {
        if (transactionHashes.empty()) return "";

        std::vector<std::string> hashes = transactionHashes;
        while (hashes.size() > 1) {
            if (hashes.size() % 2 != 0) {
                hashes.push_back(hashes.back());
            }
            std::vector<std::string> newHashes;
            for (size_t i = 0; i < hashes.size(); i += 2) {
                newHashes.push_back(sha256(hashes[i] + hashes[i + 1]));
            }
            hashes = newHashes;
        }
        return hashes.front();
    }
};

#endif
