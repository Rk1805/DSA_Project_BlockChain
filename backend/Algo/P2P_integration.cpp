#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class P2PNode;

class BloomFilter {
private:
    int size;
    int hash_count;
    vector<bool> bit_array;

    // Hash function to generate a hash based on seed value
    size_t hash(const string &item, int seed) const {
        std::hash<string> hasher;
        return (hasher(item + to_string(seed)) % size);
    }

public:
    // Constructor to initialize the Bloom Filter
    BloomFilter(int size, int hash_count)
        : size(size), hash_count(hash_count), bit_array(size, false) {}

    // Add an item to the Bloom filter
    void add(const string &item) {
        for (int i = 0; i < hash_count; ++i) {
            size_t hash_value = hash(item, i);
            bit_array[hash_value] = true;
        }
    }

    // Check if an item is in the Bloom filter
    bool check(const string &item) const {
        for (int i = 0; i < hash_count; ++i) {
            size_t hash_value = hash(item, i);
            if (!bit_array[hash_value]) {
                return false;
            }
        }
        return true;
    }
};

class P2PNode {
private:
    BloomFilter *bloom_filter;
    vector<P2PNode*> peers;

public:
    // Constructor to initialize the P2P node with a Bloom filter and peer nodes
    P2PNode(BloomFilter *filter) : bloom_filter(filter) {}

    // Add a peer node (another P2P node) to the network
    void add_peer(P2PNode* peer) {
        peers.push_back(peer);
    }

    // Receive a transaction, check if it's new, and propagate if necessary
    void receive_transaction(const string &transaction_id) {
        if (!bloom_filter->check(transaction_id)) {
            cout << "New transaction " << transaction_id << " received. Propagating..." << endl;
            bloom_filter->add(transaction_id);  // Add to the Bloom filter
            propagate(transaction_id);
        } else {
            cout << "Transaction " << transaction_id << " already seen. Not propagating." << endl;
        }
    }

    // Propagate the transaction to connected peers
    void propagate(const string &transaction_id) {
        for (auto peer : peers) {
            peer->receive_transaction(transaction_id);
        }
    }
};

int main() {
    int size = 1000;   // Bloom filter size
    int hash_count = 5; // Number of hash functions

    // Create a Bloom filter instance
    BloomFilter bloom(size, hash_count);

    // Create two P2P nodes and connect them
    P2PNode node1(&bloom);
    P2PNode node2(&bloom);

    // Add node1 as a peer to node2
    node2.add_peer(&node1);

    // Simulate receiving a new transaction
    node2.receive_transaction("tx001"); // Propagates since it's new
    node2.receive_transaction("tx002");
    node2.receive_transaction("tx004");
    node2.receive_transaction("tx001"); // Does not propagate since it's already seen

    return 0;
}
