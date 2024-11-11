

```
   ____ ______   ______ _____ ___  _   _ _____  __ 
  / ___|  _ \ \ / /  _ \_   _/ _ \| \ | |_ _\ \/ / 
 | |   | |_) \ V /| |_) || || | | |  \| || | \  /  
 | |___|  _ < | | |  __/ | || |_| | |\  || | /  \  
  \____|_| \_\|_| |_|    |_| \___/|_| \_|___/_/\_\ 
```

Cryptonix is a simple blockchain-based web application developed using C++, Javascript, Node.js, React.js, Firebase, and OpenSSL. The application is designed to provide a basic understanding of blockchain concepts such as transaction signing, Merkle trees, and a proof-of-work consensus algorithm. Users can create accounts, make secure transactions, view transaction history, and check balances, all hosted on a blockchain secured with cryptographic methods.

## Table of Contents
- [Features](#features)
- [Technologies](#technologies)
- [Setup and Installation](#setup-and-installation)
- [Usage](#usage)
- [Blockchain Implementation Details](#blockchain-implementation-details)
- [Future Improvements](#future-improvements)


## Features
- **User Authentication:** Users can sign up, log in, and securely access their accounts.
- **Transaction Functionality:** Users can make transactions, which are securely signed and verified.S
- **Blockchain Integrity:** Each block contains multiple transactions and is hashed using SHA-256, forming a secure chain with Merkle trees for efficient transaction hashing.
- **Proof of Work:** Blocks are added based on a proof-of-work consensus, which secures the chain by requiring computational effort to validate blocks.
- **Transaction History and Balance:** Users can view their transaction history and balance within the application dashboard.

## Technologies
- **Backend:** C++ (core blockchain logic, transaction handling), Node.js (REST API)
- **Frontend:** Node.js (React for UI)
- **Database:** Firebase Firestore (user and transaction data)
- **Cryptography:** OpenSSL (SHA-256, signature generation and verification)
- **Hashing and Consensus:** Merkle trees and proof-of-work consensus algorithm


## Setup and Installation

### Prerequisites
- **Node.js** and **npm**
- **OpenSSL**
- **Firebase** account for authentication and database setup

### Installation Steps

1. **Clone the Repository:**
   ```bash
   git clone https://github.com/Rk1805/DSA_Project_BlockChain.git
   ```

2. **Install Backend Dependencies:**
   ```bash
   npm install
   ```

3. **Install Frontend Dependencies:**
   Navigate to the frontend directory and run:
   ```bash
   npm install
   ```

4. **Configure Firebase:**
   - Set up Firebase for user authentication and Firestore database.
   - Add your Firebase configuration in `firebase.js`.

5. **Run Backend:**
   ```bash
   npm start
   ```

6. **Run Frontend:**
   ```bash
   npm run dev
   ```

---

## Usage

1. **Sign Up and Login:**
   - Create a user account via the signup page.
   - Log in with registered credentials to access the dashboard.

2. **Dashboard Features:**
   - **Make a Transaction**: Specify recipient, amount, and initiate transactions.
   - **Transaction History**: View a history of previous transactions.
   - **Get Balance**: Check current balance based on transactions in the blockchain.

---

## Blockchain Implementation Details

- **Merkle Tree**: Each block contains a Merkle root of transaction hashes, ensuring integrity and efficient verification.
- **Proof of Work**: Blocks are added only after a valid proof-of-work hash is found, helping secure the chain.
- **Transaction Signing**: Transactions are cryptographically signed using OpenSSL, ensuring sender verification.
- **Block Structure**: Each block contains a previous block hash, Merkle root, and the current block’s transactions, forming a linked list structure.

---

## Future Improvements
- Implementing additional cryptographic mechanisms for enhanced security.
- Improving consensus efficiency and experimenting with other algorithms.
- Expanding functionality to handle multiple cryptocurrencies or digital assets.
- Adding an enhanced front-end UI for better user experience.

---

This application provides a simplified but functional implementation of blockchain concepts, focusing on secure transactions and data integrity. For more in-depth details, refer to the individual files in each directory.

--- 

