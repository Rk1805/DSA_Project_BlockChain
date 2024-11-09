#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
std::string sender;
std::string receiver;
float amount;
std::string signature;

public:
Transaction() = default; // Default constructor
Transaction(const std::string& sender, const std::string& receiver, float amount);

std::string getData() const; // Get data to sign
std::string getSender() const;
std::string getReceiver() const;
float getAmount() const;
void setSignature(const std::string& sig);
bool signTransaction(const std::string& privateKey);
std::string getSignature() const;
bool verifySignature(const std::string& publicKey) const;
};

#endif // TRANSACTION_H
