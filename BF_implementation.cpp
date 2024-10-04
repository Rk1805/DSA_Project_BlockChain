#include <iostream>
#include <vector>
#include <string>
#include <functional>

using namespace std;

class BloomFilter {
private:
    int size;                   // Size of the bit array (m)
    int hash_count;             // Number of hash functions (k)
    vector<bool> bit_array;     // The bit array to store the filter

    // Hash function to generate a hash based on seed value
    unsigned int hash(const string &item, int seed) const {
       std::hash<string> hasher;
        return (hasher(item + to_string(seed)) % size);
    }

public:
    // Constructor to initialize Bloom Filter with size and hash count
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
                return false; // Definitely not in the filter
            }
        }
        return true; // Possibly in the filter (false positive possible)
    }
};

int main() {
    int size = 1000;   // Bloom filter size
    int hash_count = 5; // Number of hash functions

    // Create a Bloom filter instance
    BloomFilter bloom(size, hash_count);

    // Sample transactions (e.g., transaction IDs)
    vector<string> transactions = {"tx123", "tx456", "tx789"};

    // Add transactions to the Bloom filter
    for (const auto &tx : transactions) {
        bloom.add(tx);
    }
  for (const auto &tx : transactions) {
        bloom.add(tx);
    }



    // Check if a new transaction exists in the Bloom filter
    string new_transaction = "tx999";
    if (bloom.check(new_transaction)) {
        cout << "Transaction might exist (do not propagate)." << endl;
    } else {
        cout << "Transaction is new (propagate)." << endl;
    }

    return 0;
}
