#include "Wallet.h"
#include "Transaction.h"
#include <vector>

int main() {
    Wallet alice("Alice");
    Wallet bob("Bob");

    // Alice sends funds to Bob
    Transaction tx = alice.sendFunds(bob, 50.0);

    // Update balances
    std::vector<Transaction> txList = {tx};
    alice.updateBalance(txList);
    bob.updateBalance(txList);

    // Print wallet data
    alice.printWalletData();
    bob.printWalletData();

    return 0;
}
