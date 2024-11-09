#ifndef PROOFOFWORK_H
#define PROOFOFWORK_H

#include <string>
#include <sstream>
#include <iomanip>
#include "SHA256.h"

class ProofOfWork {
public:
    int difficulty;

    ProofOfWork(int difficultyLevel) : difficulty(difficultyLevel) {}

    std::string mineBlock(int& nonce, const std::string& data, const std::string& prevHash) {
        std::string hash;
        std::string target(difficulty, '0'); // Target hash starts with 'difficulty' number of zeros
        
        do {
            nonce++;
            hash = calculateHash(data, prevHash, nonce);
        } while (hash.substr(0, difficulty) != target);

        return hash;
    }

private:
    std::string calculateHash(const std::string& data, const std::string& prevHash, int nonce) {
        std::ostringstream ss;
        ss << data << prevHash << nonce;
        return sha256(ss.str());
    }
};

#endif
