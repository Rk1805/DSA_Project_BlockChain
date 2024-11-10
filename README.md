      
# Project Overview
This project implements a simplified blockchain system focusing on core components such as:

- Merkle Trees for transaction verification.
- Bloom Filters for efficient block and transaction propagation.
- Consensus Algorithms like Proof of Work (PoW) and Proof of Stake (PoS) for securing the blockchain.
- The aim of this project is to provide a foundational understanding of blockchain technology by building these essential features from scratch.

# Features
### 1. Blockchain Data Structure
Core structure of the blockchain, including:
Block creation with a header, list of transactions, and hash.
Genesis block initialization.
Hash linking between blocks to ensure chain immutability.
### 2. Merkle Tree for Transaction Verification
Merkle Tree implementation for secure and efficient verification of transactions within blocks.
Computes the Merkle root of transactions and provides a proof mechanism to verify if a transaction belongs to a block.
### 3. Bloom Filter for Efficient Block Propagation
Implements a Bloom filter for fast verification of whether a transaction exists in a block, optimizing the block propagation process across the network.
### 4. Consensus Algorithms
Proof of Work (PoW): Simulates a mining process where nodes solve cryptographic puzzles to secure the blockchain.
Proof of Stake (PoS): Implements validator selection based on coin holdings to create new blocks, ensuring energy-efficient consensus.
### 5. Transaction and Block Propagation
Simulates a basic peer-to-peer (P2P) network for transaction and block propagation between nodes.
Integrates Bloom filters to improve network efficiency.

Contributors:-
1.) Rishi Kaneria
2.) Sahilpreet Singh
3.) Saher Dev
4.) Sahil Narkhede 
5.) Akshay
