import { exec } from "child_process";
import fetch from "node-fetch"
import { db } from "../config/firebase.js"; // Ensure this imports your Firestore instance
import { collection,addDoc,getDocs, query, where} from "firebase/firestore";
import fs from "fs";
import express from "express";

const app = express();
app.use(express.json());

// Function to add user data to Firestore
export const getHistory = async (req, res) => {
    try {
      const data = fs.readFileSync('./Algo/chain.json', 'utf8');
      const blockchain = JSON.parse(data);
  
      // Collect all transactions with the required fields
      const transactionsList = blockchain.chain.flatMap(block => 
        block.transactions.map(tx => ({
          amount: tx.amount,
          sender: tx.sender,
          receiver: tx.receiver,
          date: tx.date // Assuming each block has a timestamp field
        }))
      );

      console.log(transactionsList);
      // Send the transactions array as JSON response
      res.status(200).json({
        message: "Transactions retrieved successfully",
        transactions: transactionsList
      });
  
    } catch (err) {
      console.error("Error reading or parsing JSON file:", err);
      res.status(500).json({
        message: "Error reading blockchain data",
        error: err.message
      });
    }
  };
