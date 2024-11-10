#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include "Transaction.h"
#include "MerkleTree.h"
#include "ProofOfWork.h"

using namespace std;

class Block {
public:
    string previousBlockHash;         //Stores the hash of the previous block, linking this block to the previous one.
    string merkleRoot;                //Holds the Merkle root hash of all transactions in this blo
    int nonce;                        //Used in proof-of-work to create a valid block hash.
    string hash;                      //Stores the hash of this block.
    vector<Transaction> transactions; //A vector containing the transactions for this block.

    /* CONTSTRUCTOR*/
    Block(const vector<Transaction>& transactions, const string& prevHash)
        : previousBlockHash(prevHash), transactions(transactions), nonce(0), hash("") {
        MerkleTree merkleTree;    //creates a Merkle tree object.
        merkleRoot = merkleTree.calculateMerkleRoot(getTransactionHashes());
        // is set by calculating the Merkle root hash from all transaction hashes using calculateMerkleRoot.
    }

    string calculateBlockHash() const {
        return sha256(previousBlockHash + merkleRoot + to_string(nonce));
    }

    void setNonce(int newNonce) { nonce = newNonce; }//setting the nonce 
/* basically nonce is a type of iterator for guess to assume how many guesses it took to solve the pow puzzle */
    void setHash(const string& newHash) { hash = newHash; }

    string getData() const {//will be used in proof of work to solve the puzzle
        return previousBlockHash + merkleRoot;
    }

private:
/*    
This  function returns a vector of SHA-256 hashes for each transaction in the block:
It iterates over transactions.
For each transaction, it calls transaction.toString() to get a string representation and then hashes it with sha256.
The resulting list of hashes is returned.
*/
    vector<string> getTransactionHashes() const {//to get the transaction hashes to be used in merkle tree
        vector<string> hashes;
        for (const auto& transaction : transactions) {
            hashes.push_back(sha256(transaction.toString()));
        }
        return hashes;
    }
};

#endif
