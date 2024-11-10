#ifndef MERKLETREE_H
#define MERKLETREE_H

#include "SHA256.h"
#include <vector>

using namespace std;

//merkle implementation in blocks

/*
A Merkle Tree is a data structure used to efficiently summarize and verify large sets of data, such as transactions in a blockchain. 
By hashing pairs of data iteratively, it creates a single root hash (Merkle root) that represents the entire dataset.
Any change in the data affects the Merkle root, making it easier to detect tampering or inconsistencies.
*/

class MerkleTree {
public:
    string calculateMerkleRoot(const vector<string> &transactionHashes) {
        /*
        Merkle tree makes use of the transaction history to create merkle root hash
        */
        if (transactionHashes.empty()) return "";//base case 

        vector<string> hashes = transactionHashes;//local variable to help out w the process 
        while (hashes.size() > 1) { //if only one then there's no need to append
            if (hashes.size() % 2 != 0) {
                /*If the hashes vector has an odd number of elements, 
                it appends the last element again to make the count even.
                This is required because Merkle Trees need pairs of hashes.*/
                
                hashes.push_back(hashes.back());
            }
            vector<string> newHashes;//creation of a new vector of hashes 
            for (size_t i = 0; i < hashes.size(); i += 2) {
                newHashes.push_back(sha256(hashes[i] + hashes[i + 1]));//iterates over pair( Concatenates two adjacent hashes )
            }
            //sort of a merge sort 
            hashes = newHashes;
        }
        return hashes.front(); //return the one and only root 
    }
};

#endif
