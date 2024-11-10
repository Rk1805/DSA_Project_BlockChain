#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
using namespace std;

class Transaction {
public:
    string sender;
    string receiver;
    double amount;

    //constructor
    Transaction(string sender, string receiver, double amount)
        : sender(sender), receiver(receiver), amount(amount) {}

    string toString() const {
        return sender + "->" + receiver + ": " + to_string(amount);
    }
};

#endif
