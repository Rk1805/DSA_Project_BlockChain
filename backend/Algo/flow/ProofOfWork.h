#ifndef PROOFOFWORK_H
#define PROOFOFWORK_H

#include <string>
#include <sstream>
#include <iomanip>
#include "SHA256.h"

using namespace std ;

class ProofOfWork {
public:
    int difficulty;// defining the difficulty of level

    ProofOfWork(int difficultyLevel) : difficulty(difficultyLevel) {} //constructor to initialise the proof od work's difficulty for the puzzle'

   // Mine block helps in the creation of highly difficult to solve puzzle(computationally) delaying the process of addition of any block preventing any malicous activity
    string mineBlock(int& nonce, const string& data, const string& prevHash) {
        string hash; //creating of a hash
        string target(difficulty, '0'); // Target hash starts with 'difficulty' number of zeros 
        /*  target = "00" */

        /*    if hash doesnt have substring 00 in the substring , keep increasing the nonce which might be in a range of 0 to 100 */
        do {
            nonce++;
            hash = calculateHash(data, prevHash, nonce); // to create the hash data and the prev hash with the nonce is taken 
        } while (hash.substr(0, difficulty) != target);

        return hash;
    }

private:
    string calculateHash(const string& data, const string& prevHash, int nonce) {
        ostringstream ss;
        ss << data << prevHash << nonce; //string stream is used to append these terms to create a single term and then furthr move on to creating this string to a hash
        return sha256(ss.str());// calling the sha 256 function to create a 256 byte length which is 32 characters hash at maximum
    }
};

#endif
