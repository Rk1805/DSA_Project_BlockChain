#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
public:
    std::string sender;
    std::string receiver;
    double amount;

    Transaction(std::string sender, std::string receiver, double amount)
        : sender(sender), receiver(receiver), amount(amount) {}

    std::string toString() const {
        return sender + "->" + receiver + ": " + std::to_string(amount);
    }
};

#endif
