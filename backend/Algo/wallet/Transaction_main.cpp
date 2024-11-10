#include "Transaction.h"
#include <iostream>
#include <string>
#include<fstream>
#include<sstream>
using namespace std;
std::string readPrivateKeyFromFile(const std::string& privateKeyFile) {
    // Expanding '~' to home directory manually
    const char* homeDir = std::getenv("HOME");  // Get home directory
    if (homeDir) {
        std::string expandedPath = privateKeyFile;
        size_t pos = expandedPath.find("~");
        if (pos != std::string::npos) {
            expandedPath.replace(pos, 1, homeDir);  // Replace ~ with home directory
        }
        std::ifstream keyFile(expandedPath);  // Open the expanded path

        if (!keyFile.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << keyFile.rdbuf();
        return buffer.str();  // Return the private key as a string
    }
    return "";
}

std::string readPublicKeyFromFile(const std::string& publicKeyFile) {
    // Expanding '~' to home directory manually
    const char* homeDir = std::getenv("HOME");  // Get home directory
    if (homeDir) {
        std::string expandedPath = publicKeyFile;
        size_t pos = expandedPath.find("~");
        if (pos != std::string::npos) {
            expandedPath.replace(pos, 1, homeDir);  // Replace ~ with home directory
        }
        std::ifstream keyFile(publicKeyFile);  // Open the expanded path

        if (!keyFile.is_open()) {
            return "";
        }

        std::stringstream buffer;
        buffer << keyFile.rdbuf();
        return buffer.str();  // Return the private key as a string
    }
    return "";
}

int main() {
    std::string sender, receiver, privateKey;
    float amount;

    // Prompt the user for input
    std::cin >> sender;

    std::cin >> receiver;

    std::cin >> amount;

    std::cin.ignore();  // To ignore any extra newline character left by previous input.

    std::string privateKeyFile = "~/.ssh/id_rsa";
    std::string publicKeyFile = "./publicKey.pem";

    // Read the private key from the file
    privateKey = readPrivateKeyFromFile(privateKeyFile);
    if (privateKey.empty()) {
        return 1;  // Exit if the private key could not be loaded
    }
    // Create the transaction object
    Transaction transaction(sender, receiver, amount);

    // Sign the transaction with the private key
    if (!transaction.signTransaction(privateKey)) {
        return 1;  // Exit if signing fails
    }

    // Optionally, you can also verify the signature by using a public key
    std::string publicKey;  // You can add code to accept a public key for verification if needed
    publicKey = readPublicKeyFromFile(publicKeyFile);
    if (publicKey.empty()) {
        return 1;  // Exit if the private key could not be loaded
    }

    if (transaction.verifySignature(publicKey)) {
        std::cout << transaction.getSignature() << std::endl;
    } else {
        return 0;
    }

    return 0;
}
