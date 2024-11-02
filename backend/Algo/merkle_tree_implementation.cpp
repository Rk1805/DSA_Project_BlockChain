#include <iostream>
#include <vector>
#include <string>
#include <openssl/sha.h>

// Function to perform SHA256 hashing on a string
std::string sha256(const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, data.c_str(), data.size());
    SHA256_Final(hash, &sha256);
    
    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }
    return ss.str();
}

// Merkle Tree Node
struct MerkleNode {
    std::string hash;
    MerkleNode* left;
    MerkleNode* right;

    MerkleNode(const std::string& h) : hash(h), left(nullptr), right(nullptr) {}
};

// Merkle Tree Class
class MerkleTree {
public:
    MerkleNode* root;

    // Constructor that builds the Merkle Tree from a list of transactions
    MerkleTree(const std::vector<std::string>& transactions) {
        root = buildMerkleTree(transactions);
    }

    // Function to build the Merkle Tree recursively
    MerkleNode* buildMerkleTree(const std::vector<std::string>& transactions) {
        if (transactions.size() == 1) {
            return new MerkleNode(sha256(transactions[0]));
        }

        std::vector<MerkleNode*> nodes;
        for (const std::string& tx : transactions) {
            nodes.push_back(new MerkleNode(sha256(tx)));
        }

        while (nodes.size() > 1) {
            if (nodes.size() % 2 != 0) {
                // If odd, duplicate the last element
                nodes.push_back(new MerkleNode(nodes.back()->hash));
            }

            std::vector<MerkleNode*> newLevel;
            for (size_t i = 0; i < nodes.size(); i += 2) {
                std::string combinedHash = sha256(nodes[i]->hash + nodes[i + 1]->hash);
                MerkleNode* parentNode = new MerkleNode(combinedHash);
                parentNode->left = nodes[i];
                parentNode->right = nodes[i + 1];
                newLevel.push_back(parentNode);
            }
            nodes = newLevel;  // Move to the next level
        }

        return nodes[0];  // The last remaining node is the root
    }

    // Function to get the root hash of the Merkle Tree
    std::string getRootHash() const {
        return root ? root->hash : "";
    }
};

// Main function to test the Merkle Tree implementation
int main() {
    std::vector<std::string> transactions = {
        "tx1", "tx2", "tx3", "tx4", "tx5"
    };

    MerkleTree merkleTree(transactions);

    std::cout << "Merkle Root Hash: " << merkleTree.getRootHash() << std::endl;

    return 0;
}
