#include <iostream>          // For input and output
#include <string>            // For using the string class
#include <sstream>           // For stringstream, used in converting hash to a hex string
#include <iomanip>           // For setting the output format (like setting hex width and fill)
#include <openssl/sha.h>     // Assuming OpenSSL for SHA-256(should be downloaded on the system)

using namespace std;

// Utility function to calculate SHA-256 hash
string sha256(const string &input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];//defines the length of 32 bytes for the hash
    SHA256_CTX sha256;//a struct representing the context for the SHA-256 operations
    /*  Following are :
    OpenSSL functions that initialize, process, and finalize the hash computation
    */

    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
     /*
     Ensuring ss << hex is only applied once (if need be )outside the loop, as repeated setting in the loop could cause unintended formatting behavior.
     */

    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i) {
        /* Converts the hash from raw bytes to a hex string */
        ss << hex << setw(2) << setfill('0') << (int)hash[i];
    }

    return ss.str(); //Converts stringstream content to a string and returns it.
}

// Function to perform Proof of Work on a block
string proofOfWork(Block &block, int difficulty) {
    /*
     Sets the target string to contain difficulty number of '0' characters, which is the condition the hash must meet 
    */

    string target(difficulty, '0');  // Difficulty target string with leading zeros
    string hash;
    
    // Continuously increment the nonce until a valid hash is found
    while (true) {

        /*
        Continuously concatenates the block's data and nonce, hashes it, and checks if the hash starts with a number of zeroes equal to the difficulty level.
        */

        // Combine block data and nonce to form a unique string
        string blockData = block.getData() + to_string(block.nonce);
        
        // Hash the block data
        hash = sha256(blockData);
        
        // Check if the hash meets the difficulty level (starts with target leading zeros)
        if (hash.substr(0, difficulty) == target) {
            break;  // Valid hash found
        }
        
        // Increment the nonce and try again since this ensures a new hash each time
        block.nonce++;
        
    }
    
    // Return the valid hash
    return hash;
}

/*
Some important points: 
1) If difficulty is greater than SHA-256’s length (64 characters in hex), the loop may run indefinitely.

*/