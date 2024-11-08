#include <iostream>
#include <vector>
#include <string>
#include <openssl/evp.h>
#include <iomanip>
#include <sstream>

// Function to perform SHA256 hashing on a string using OpenSSL EVP API
std::string sha256(const std::string& data) {
    unsigned char hash[EVP_MAX_MD_SIZE];  // EVP_MAX_MD_SIZE is the maximum size for all digests
    unsigned int hashLength;  // Store the actual length of the hash

    EVP_MD_CTX* context = EVP_MD_CTX_new();  // Create a new digest context
    if (context == nullptr) return "";  // Error check

    if (EVP_DigestInit_ex(context, EVP_sha256(), nullptr) != 1) return "";  // Initialize digest
    if (EVP_DigestUpdate(context, data.c_str(), data.size()) != 1) return "";  // Update digest with data
    if (EVP_DigestFinal_ex(context, hash, &hashLength) != 1) return "";  // Finalize the digest and get length

    EVP_MD_CTX_free(context);  // Clean up the digest context

    std::stringstream ss;
    for (unsigned int i = 0; i < hashLength; i++) {
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
