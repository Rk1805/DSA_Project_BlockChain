#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include "Block.h"
#include "ProofOfWork.h"
#include <vector>
using namespace std;

//merkle and pow integration 

class Blockchain {
private:
    vector<Block> chain;// a vector of blocks known as "chain"  block1<--|block2<--|block3<--|block4
    int difficulty;//setting up the difficulty for the blockchain

public:
    /*CONSTRUCTOR*/
    Blockchain(int diff = 4) : difficulty(diff) {
        /*
        empty list of transactions with prev hash as zero and automatic difficulty of 4 
        i.e 4 leading zeroes
        */
        chain.emplace_back(vector<Transaction>(), "0"); // Genesis block
    }

    void addBlock(const vector<Transaction>& transactions,string prevBlockHash) {
        Block newBlock(transactions, prevBlockHash);//creation of a new block with the history of transactions and prev block hash

        // Proof of Work
        ProofOfWork pow(difficulty);//initialising pow 
        int nonce = 0;
        string minedHash = pow.mineBlock(nonce, newBlock.getData(), prevBlockHash);//calling out to the function to solve the puzz;e

        newBlock.setNonce(nonce);//setting up new nonce,hash in block
        newBlock.setHash(minedHash);
        chain.push_back(newBlock);
    }

    const vector<Block>& getChain() const {
        return chain;
    }
};

#endif
