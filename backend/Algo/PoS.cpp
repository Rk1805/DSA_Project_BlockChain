#include <iostream>
#include <vector>
#include <map>
#include <cstdlib>   // For random number generation
#include <ctime>
#include <algorithm>
using namespace std;


// Block structure
class Block {
public:
    int index;
    string previousHash;
    string data;
    string validator;  // The chosen validator for this block

    Block(int idx, string prevHash, string data, string validator)
        : index(idx), previousHash(prevHash), data(data), validator(validator) {}
};

// Validator structure
class Validator {
public:
    string name;
    int stake;  // Amount of tokens held by the validator

    Validator(string name, int stake) : name(name), stake(stake) {}
};

// Blockchain structure
class Blockchain {
public:
    vector<Block> chain;
    vector<Validator> validators;
    map<string, int> validatorStakes;  // Track validator stakes by name
    // int stakeThreshold; // Minimum stake required to add a block

    Blockchain() {
        // Create the genesis block
        /*
        GENESIS BLOCK is the initial block of the blockchain
        */
        chain.push_back(Block(0, "0", "Genesis Block", "None"));
        // stakeThreshold = 10; // Set a threshold for stakes (can be adjusted)
    }

    // Add validators to the network
    void addValidator(string name, int stake) {
        validators.push_back(Validator(name, stake));
        validatorStakes[name] = stake;
    }

    // Choose a validator based on their stake
    // (higher stake = higher chance)
    string chooseValidator() {
        int totalStake = 0;
        for (const auto& v : validators) {
            totalStake += v.stake;
        }

        // Choose a random number and assign the validator based on their stake proportion
        int randomSelection = rand() % totalStake;
        
        int cumulativeStake = 0;
        for (const auto& v : validators) {
            cumulativeStake += v.stake;
            if (randomSelection < cumulativeStake) {
                return v.name;  // Validator chosen
            }
        }
        return ""; // Shouldn't reach here
    }

    // Malicious activity detection
    bool isMalicious(string data, string validator) {
        // Check if the validator's stake is below the threshold
       
        if (validatorStakes[validator] < stakeThreshold()) {
            cout << "Malicious activity detected: " << validator << " has insufficient stake." << endl;
            //  penalizeValidator(validatorName, 10);
            return true;
        }
        // Check if the data is invalid
        if (data.empty()) {
            cout << "Malicious activity detected: Invalid data." << endl;
            return true;
        }
        // Add more checks as needed
        return false;
    }
      // Function to penalize a validator for malicious activity
    void penalizeValidator(string validatorName, int penaltyAmount) {
        if (validatorStakes[validatorName] >= penaltyAmount) {
            validatorStakes[validatorName] -= penaltyAmount;
        } else {
            validatorStakes[validatorName] = 0;  // Penalty reduces stake to zero if it's too high
        }
    } 
    
    // Add a new block
    void addBlock(string data) {
        string validator = chooseValidator();  // Select the validator
        if (isMalicious(data, validator)) {
            cout << "Block addition failed due to malicious activity!" << endl;
            //  penalizeValidator(validator, 10); // Example penalty amount
            return;
        }

        Block newBlock(chain.size(), chain.back().previousHash, data, validator);
        chain.push_back(newBlock);
        
        // Display the remaining stakes of each validator after adding the block
    
    }

   

    // Simulate a malicious activity (for demonstration purposes)
    void simulateMaliciousActivity(string validatorName) {
        cout << "Malicious activity detected for " << validatorName << "!" << endl;
         cout<<"The stakeThreshold for malicious activity is: "<<stakeThreshold()<<endl;
        penalizeValidator(validatorName, 10); // Example penalty amount
    }

    int stakeThreshold() {
        if (validators.empty()) return 0;  // Avoid division by zero

        int total = totalStake();
        double averageStake = static_cast<double>(total) / validators.size();
        return static_cast<int>(averageStake * 0.25);  // Multiply average by t and return as int
    }

    // Display the blockchain (to show stakes for testing)
    void displayChain() {
        for (const auto& block : chain) {
            cout << "Block " << block.index << ": " << endl;
            cout << "  Previous Hash: " << block.previousHash << endl;
            cout << "  Data: " << block.data << endl;
            cout << "  Validator: " << block.validator << endl;
        }
        cout << "Current Stake Threshold: " << stakeThreshold() << endl;  // Display stake threshold
        displayValidatorStakes(); // Display stakes
    }
    
    //Display remainding validator's stakes
    void displayValidatorStakes() {
        cout << "Remaining Stakes of Validators:" << endl;
        for (const auto& v : validators) {
            cout << "  " << v.name << ": " << validatorStakes[v.name] << " tokens" << endl;
        }
    }
    // Function to calculate total stake
    int totalStake() {
        int total = 0;
        for (const auto& validator : validators) {
            total += validator.stake;
        }
        return total;
    }
   
};

// Main function
int main() {
    srand(time(nullptr));  // Initialize random seed for validator selection
    cout << "Enter no. of Users : ";
    int n; cin >> n; // No. of validators to be added 
    
    Blockchain blockchain;
    
    for (int i = 0; i < n; i++) {
        // Adding validators with their stake amounts
        string validator = "Validator" + to_string(i); // Convert int to string
        int token;
        cout << "Enter stake for " << validator << ": ";
        cin >> token;
        blockchain.addValidator(validator, token); 
    }

    // Simulate a malicious activity for demonstration
    blockchain.simulateMaliciousActivity("Validator2");

    // Add blocks to the blockchain
    cout << "Adding blocks to the blockchain..." << endl;
   
    
     for (int i = 0; i < n; i++) {
         //enter what data needs to be put in
         
        // int data;
        // cout << "Enter data for " << validator << ": ";
        // cin >> data;
         
        string block = "Block " + to_string(i + 1) + " Data"; // Convert int to string
        blockchain.addBlock(block);
    }

   
    // Display the blockchain
    blockchain.displayChain();

    return 0;
}

// #include <iostream>
// #include <vector>
// #include <map>
// #include <cstdlib>   // For random number generation
// #include <ctime>
// #include <algorithm>
// using namespace std;

// // Block structure
// class Block {
// public:
//     int index;
//     string previousHash;
//     string data;
//     string validator;  // The chosen validator for this block

//     Block(int idx, string prevHash, string data, string validator)
//         : index(idx), previousHash(prevHash), data(data), validator(validator) {}
// };

// // Validator structure
// class Validator {
// public:
//     string name;
//     int stake;  // Amount of tokens held by the validator

//     Validator(string name, int stake) : name(name), stake(stake) {}
// };

// // Blockchain structure
// class Blockchain {
// public:
//     vector<Block> chain;
//     vector<Validator> validators;
//     map<string, int> validatorStakes;  // Track validator stakes by name

//     Blockchain() {
//         // Create the genesis block
//         /*
//         GENESIS BLOCK is the initial block of the blockchain
//         */
//         chain.push_back(Block(0, "0", "Genesis Block", "None"));
//     }

//     // Add validators to the network
//     void addValidator(string name, int stake) {
//         validators.push_back(Validator(name, stake));
//         validatorStakes[name] = stake;
//     }

//     // Choose a validator based on their stake
//     // (higher stake = higher chance)
//     string chooseValidator() {
//         int totalStake = 0;
//         for (const auto& v : validators) {
//             totalStake += v.stake;
//         }

//         /*
//         LOGIC:
//         random selection becomes a number between zero to 1 and then it is a complete comparison to a fixed range
//         */
         
//         // Choose a random number and assign the validator based on their stake proportion
//         int randomSelection = rand() % totalStake;
        
//         /*
//         EXPLANATION:
//         let us say total stakes are 350
//         user 1 stakes are: 100;~ 0.2
//         user 2 stakes are 200;~0.49
//         user 3 stakes are 50;~0.1
//         let us say the randomSelection is 0.45 this comes with user 2's stakes therefore 
//         user 2 is the validator
//         */
        
//         int cumulativeStake = 0;
//         for (const auto& v : validators) {
//             cumulativeStake += v.stake;
//             if (randomSelection < cumulativeStake) {
//                 return v.name;  // Validator chosen
//                 //comes within this range
//             }
//         }
//         return ""; // Shouldn't reach here
//     }
//      // Malicious activity detection
//     bool isMalicious(string data, string validator) {
//         // Example criteria for malicious activity
//         // 1. Check if the validator's stake is too low to add a block
//         if (validatorStakes[validator] < 10) { // Example threshold
//             cout << "Malicious activity detected: " << validator << " has insufficient stake." << endl;
//             return true;
//         }
//         // 2. Check if the data is invalid (you can add more complex checks)
//         if (data.empty()) {
//             cout << "Malicious activity detected: Invalid data." << endl;
//             return true;
//         }
//         // Add more checks as needed
//         return false;
//     }
    
//     // Add a new block
//     void addBlock(string data) {
//         string validator = chooseValidator();  // Select the validator
//         if (isMalicious(data, validator)) {
//             cout << "Block addition failed due to malicious activity!" << endl;
//             return;
//         }

//         Block newBlock(chain.size(), chain.back().previousHash, data, validator);
//         chain.push_back(newBlock);
//     }

//     // Function to penalize a validator for malicious activity
//     void penalizeValidator(string validatorName, int penaltyAmount) {
//         if (validatorStakes[validatorName] >= penaltyAmount) {
//             validatorStakes[validatorName] -= penaltyAmount;
//         } else {
//             validatorStakes[validatorName] = 0;  // Penalty reduces stake to zero if it's too high
//         }
//     }

//     // Simulate a malicious activity (for demonstration purposes)
//     void simulateMaliciousActivity(string validatorName) {
//         cout << "Malicious activity detected for " << validatorName << "!" << endl;
//         penalizeValidator(validatorName, 10); // Example penalty amount
//     }

//     // Display the blockchain
//     void displayChain() {
//         for (const auto& block : chain) {
//             cout << "Block " << block.index << ": " << endl;
//             cout << "  Previous Hash: " << block.previousHash << endl;
//             cout << "  Data: " << block.data << endl;
//             cout << "  Validator: " << block.validator << endl;
//         }
//     }

//     // Function to calculate total stake
//     int totalStake() {
//         int total = 0;
//         for (const auto& validator : validators) {
//             total += validator.stake;
//         }
//         return total;
//     }
// };

// // Main function
// int main() {
//     srand(time(nullptr));  // Initialize random seed for validator selection
//     int n;cin>>n;//no. of blocks to be added 
    
//     Blockchain blockchain;
    
//     for(int i=0;i<n;i++){
//         // Adding validators with their stake amounts
//          string validator = "Validator" + to_string(i); // Convert int to string
//         int token; cin>>token;
//          blockchain.addValidator(validator,token); 
//     // blockchain.addValidator("Validator1", 50);  // 50 tokens
//     // blockchain.addValidator("Validator2", 30);  // 30 tokens
//     // blockchain.addValidator("Validator3", 20);  // 20 tokens
//     }
    

//     // Simulate a malicious activity for demonstration
//     blockchain.simulateMaliciousActivity("Validator2");

//     // Add blocks to the blockchain
//     cout << "Adding blocks to the blockchain..." << endl;
//     blockchain.addBlock("Block 1 Data");
//     blockchain.addBlock("Block 2 Data");
//     blockchain.addBlock("Block 3 Data");

//     // Display the blockchain
//     blockchain.displayChain();

//     return 0;
// }
