#include "Transaction.h"

// Constructor
Transaction::Transaction(const std::string& sender, const std::string& receiver, float amount)
    : sender(sender), receiver(receiver), amount(amount) {}

// Get transaction data (for signing)
std::string Transaction::getData() const {
    return sender + receiver + std::to_string(amount);
}

std::string Transaction::getSender() const { return sender; }
std::string Transaction::getReceiver() const { return receiver; }
float Transaction::getAmount() const { return amount; }
void Transaction::setSignature(const std::string& sig) { signature = sig; }
