#ifndef SHA256_H //if not defined header file
#define SHA256_H

#include <string>
#include <openssl/sha.h> //Assuming OpenSSL for SHA-256(should be downloaded on the system) open software system library scure communication over computer networks
using namespace std;

string sha256(const string &input) { //input string will contain data prev hash and nonce 
    unsigned char hash[SHA256_DIGEST_LENGTH]; //defines the length of 32 bytes for the hash
    //a struct representing the context for the SHA-256 operations
    /*  Following are :
    OpenSSL functions that initialize, process, and finalize the hash computation

     (unsigned char*)input.c_str(): Converts the input string into a const unsigned char*, which is the format SHA256 expects.

     input.size(): Specifies the length of the input.

     hash: Passes in the array to store the hash result.  
     
    */
    
    SHA256((unsigned char*)input.c_str(), input.size(), hash);

    string result;// Declare a string to store the final hexadecimal hash
    char buf[3];// Buffer to hold each byte of the hash as a two-digit hex string
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        //snprintf formats each byte (hash[i]) as a two-digit hexadecimal string (e.g., 0a, ff) 
        snprintf(buf, sizeof(buf), "%02x", hash[i]);  // Use snprintf instead of sprintf
        // Convert each byte to hex format and store it in buf
        result += buf; // Append the hex representation to the result string
    }
    return result; // Return the full hexadecimal string as the SHA-256 hash
}

#endif
