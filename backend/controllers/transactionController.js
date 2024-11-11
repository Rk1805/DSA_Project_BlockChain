import { exec } from "child_process";
import fetch from "node-fetch"
import { db } from "../config/firebase.js"; // Ensure this imports your Firestore instance
import { collection,addDoc,getDocs, query, where} from "firebase/firestore";
import net from "net";
import {spawn} from "child_process";
import fs from "fs";
import path from "path";
import express from "express";
import { fileURLToPath } from 'url';
import crypto from 'crypto';

const app = express();
app.use(express.json());

// Function to add user data to Firestore
export const signupUser = async (req, res) => {
  const { username, ip, email, password } = req.body;
  console.log(username,ip,email);
  try {
    // Add a new document with a generated ID
    const userRef = await addDoc(collection(db, "users"), {
      username: username || "",
      ip: ip || "",
      email: email || "",
      password: password || "",
    });
     // Send success response
    res.status(201).send({ id: userRef.id, message: "User data added successfully!" });
  } 
  
  catch (error) {
    console.error("Error adding user data:", error);
    // Send error response
    res.status(400).send({ error: error.message });
  }
};


// Controller to handle user login with email and password validation
export const loginUser = async (req, res) => {
  const { email, password } = req.body;
  console.log(email,password);
  try {
    // Query Firestore to check if the email exists
    const usersRef = collection(db, "users");
    const emailQuery = query(usersRef, where("email", "==", email));
    const querySnapshot = await getDocs(emailQuery);

    if (querySnapshot.empty) {
      // No user found with the provided email
      return res.status(404).send({ message: "Email not found. Please sign up first." });
    } 

    // Check if the password matches the one in Firestore
    const userDoc = querySnapshot.docs[0];
    const userData = userDoc.data();
    
    if (userData.password === password) {
      // Password is correct, proceed with login
      console.log(userData);
      return res.status(200).send({ id:userData.username });
    } else {
      // Password is incorrect
      return res.status(401).send({ message: "Incorrect password. Please try again." });
    }
  } catch (error) {
    console.error("Error logging in user:", error);
    res.status(500).send({ error: error.message });
  }
};

// Endpoint to retrieve all peer IP addresses
export const getAllPeers = async (req, res) => {
  try {
    console.log("Fetching all peers...");
    const peersSnapshot = await getDocs(collection(db, "users"));
    if (peersSnapshot.empty) {
        console.log("No peers found in database.");
        res.json({ peers: [] });
        return;
    }

    const peerIPs = peersSnapshot.docs.map((doc) => {
        const ip = doc.data().ip;
        console.log("IP:", ip);
        return ip;
    });

    res.json({ peers: peerIPs });
  } catch (error) {
    console.error("Error fetching peers:", error);
    res.status(500).send("Failed to retrieve peer IP addresses");
  }
};

export const broadcast = async (req, res) => {
  console.log("Full request body:", req.body);
  const {data, message}=req.body
  console.log("Received broadcast_data:" );
  const peersSnapshot = await getDocs(collection(db, "users"));
    if (peersSnapshot.empty) {
        console.log("No peers found in database.");
        return;
    }

    const peerIPs = peersSnapshot.docs.map((doc) => {
        const ip = doc.data().ip;
        console.log("IP:", ip);
        return ip;
    });
    console.log(peerIPs);
  const onlinePeers = [];
  const jsonData = {message:message,data:data}; 

  await Promise.all(
    peerIPs.map((ip) => {
      return new Promise((resolve) => {
        const targetPort = 6001;
        const client = new net.Socket();
        const timeout = setTimeout(() => {
          console.error(`Timeout on peer ${ip}`);
          client.destroy(); // Close client if timeout occurs
          resolve();
        }, 200); // 0.2-second timeout
  
        client.connect(targetPort, ip, () => {
          console.log(`Connected to peer at ${ip}:${targetPort}`);

          // Ensure jsonData is defined and is a string
          const jsonDataString = JSON.stringify(jsonData) || '';
          
          client.write(jsonDataString); // Send JSON data as a string
        });
  
        client.on("data", (response) => {
          clearTimeout(timeout); // Clear timeout if data received
          console.log(`Received response from peer ${ip}:`, response.toString());
          onlinePeers.push(ip);
          client.destroy();
          resolve();
        });
  
        client.on("error", (err) => {
          clearTimeout(timeout);
          console.error(`Connection error with peer ${ip}:`, err.message);
          client.destroy();
          resolve();
        });
  
        client.on("close", () => {
          console.log(`Connection closed with peer ${ip}`);
        });
      });
    })
  );

  res.status(200).send({
    message: "Broadcast complete",
    onlinePeers,
  });
};

export const singlebroadcast = async (req, res) => {
  console.log("Full single request body:", req.body);
  const {data,message} = req.body;
  console.log("Received single broadcast_data:", data);
  const sender=data.sender
  const to=data.to
  const amount=data.amount
  const signature=data.signature
  const publicKey=data.publicKey

  // Fetch all peer IPs from Firestore
  const peersSnapshot = await getDocs(collection(db, "users"));
  if (peersSnapshot.empty) {
    console.log("No peers found in database.");
    return res.status(404).send("No peers found.");
  }

  let peerIPs = peersSnapshot.docs.map((doc) => doc.data().ip);
  console.log("Initial peer IPs:", peerIPs);
  const jsonData = {
    sender,
    to,
    amount,
    signature,
    publicKey
  };

  const broadcastToPeer = (ip) => {
    return new Promise((resolve) => {
      const targetPort = 6001;
      const client = new net.Socket();
      const timeout = setTimeout(() => {
        console.error(`Timeout on peer ${ip}`);
        client.destroy();
        resolve(false); // Indicate that this peer was not online
      }, 1000); // 0.2-second timeout

      client.connect(targetPort, ip, () => {
        console.log(`Connected to peer at ${ip}:${targetPort}`);
        client.write(JSON.stringify({data:jsonData,message:"transaction"}));
      });

      client.on("data", (response) => {
        clearTimeout(timeout);
        console.log(`Received response from peer ${ip}:`, response.toString());
        client.destroy();
        resolve(true); // Successful broadcast
      });

      client.on("error", (err) => {
        clearTimeout(timeout);
        console.error(`Connection error with peer ${ip}:`, err.message);
        client.destroy();
        resolve(false); // This peer is offline
      });

      client.on("close", () => {
        console.log(`Connection closed with peer ${ip}`);
      });
    });
  };

  // Try broadcasting to random peers until one is online
  while (peerIPs.length > 0) {
    const randomIndex = Math.floor(Math.random() * peerIPs.length);
    const ip = peerIPs.splice(randomIndex, 1)[0]; // Remove the chosen IP from the list

    const success = await broadcastToPeer(ip);
    if (success) {
      return res.status(200).send({ message: `Broadcast successful to peer ${ip}` });
    }
  }

  // If no peers are online
  res.status(500).send({ message: "No peers were online to receive the broadcast." });
};


export const recieveTransaction = (req, res) => {
  console.log("req body",req.body);
  const { data,message } = req.body;
  console.log(data,message)

  const __filename = fileURLToPath(import.meta.url);
  const __dirname = path.dirname(__filename);
  const filePath = path.join(__dirname, "../Algo/chain.json");

  if(message=="block"){
    fs.readFile(filePath, "utf8", (err, fileData) => {
      if (err) {
        console.error("Error reading chain.json:", err);
        return res.status(500).json({ error: "Error reading chain file" });
      }
  
      let blockchain;
      try {
        blockchain = JSON.parse(fileData);
  
        // Add the new block to the blockchain
        blockchain.chain.push(data);
  
        // Write the updated blockchain back to chain.json
        fs.writeFile(filePath, JSON.stringify(blockchain, null, 2), "utf8", (writeErr) => {
          if (writeErr) {
            console.error("Error writing to chain.json:", writeErr);
            return res.status(500).json({ error: "Error updating chain file" });
          }
  
          res.json({
            message: "Block added successfully!",
            block: data
          });
        });
      } catch (parseErr) {
        console.error("Error parsing chain.json:", parseErr);
        return res.status(500).json({ error: "Error parsing chain file" });
      }
    });
  }
  else if(message=="transaction"){
    console.log("transaction", data);
    // const transactionData = data.sender + data.to + data.amount;
    // if (!verifySignature(data.publicKey, data.signature, transactionData)) {
    //   console.log("Invalid signature");
    //   return res.status(400).json({ error: "Invalid signature" });
    // }
    // console.log("verification successful!");
    // Read the blockchain to get the last block's hash
    fs.readFile(filePath, "utf8", (err, fileData) => {
      if (err) {
        console.error("Error reading chain.json:", err);
        return res.status(500).json({ error: "Error reading chain file" });
      }

      let blockchain;
      try {
        blockchain = JSON.parse(fileData);

        // Check if the chain has blocks
        if (blockchain.chain.length === 0) {
          return res.status(500).json({ error: "Blockchain is empty" });
        }

        // Get the last block's hash
        const lastBlock = blockchain.chain[blockchain.chain.length - 1];
        console.log(lastBlock)
        const previousBlockHash = lastBlock.hash;
        console.log(previousBlockHash);

        // Continue with transaction processing using previousBlockHash
        const cppProcess = spawn("./Algo/flow/main"); // Path to your compiled C++ executable

        cppProcess.stdin.write(data.sender + '\n');
        cppProcess.stdin.write(data.to + '\n');
        cppProcess.stdin.write(data.amount + '\n');
        cppProcess.stdin.write(previousBlockHash + '\n');
        cppProcess.stdin.end();

        let output = ''; // Accumulate all output here

        cppProcess.stdout.on('data', (data) => {
          output += data.toString();
        });

        cppProcess.stderr.on('data', (data) => {
          console.error(`stderr: ${data}`);
        });

        cppProcess.on('close', (code) => {
          if (code === 0) {
            const outputLines = output.trim().split('\n');
            console.log(outputLines);
            if (outputLines.length >= 3) {
              const [newpreviousBlockHash,merkleRoot, hash, nonce] = outputLines;

              let amt = parseFloat(data.amount);
              const newBlock = {
                previousBlockHash,
                merkleRoot,
                hash,
                nonce,
                transactions: [
                  {
                    sender: data.sender,
                    receiver: data.to,
                    amount: amt,
                    date: new Date().toLocaleDateString(),
                  }
                ]
              };

                res.json({
                  message: "block",
                  block: newBlock
                });
            } else {
              res.status(500).json({ error: 'Insufficient output from C++ process' });
            }
          } else {
            res.status(500).json({ error: 'Failed to sign transaction' });
          }
        });
      } catch (parseErr) {
        console.error("Error parsing chain.json:", parseErr);
        return res.status(500).json({ error: "Error parsing chain file" });
      }
    });
  }
  else if(message=="update") {
    fs.readFile(filePath, "utf8", (err, fileData) => {
      if (err) {
        console.error("Error reading chain.json:", err);
        return res.status(500).json({ error: "Error reading chain file" });
      }
  
      let blockchain;
      try {
        blockchain = JSON.parse(fileData);
        res.json(blockchain.chain);
      } catch (parseErr) {
        console.error("Error parsing chain.json:", parseErr);
        return res.status(500).json({ error: "Error parsing chain file for update" });
      }
    });
  }
};


export const signTransaction = (req, res) => {

  const { sender, to, amount } = req.body;

  const __filename = fileURLToPath(import.meta.url);
  const __dirname = path.dirname(__filename);
  const filePath = path.join(__dirname, "../publicKey.pem");
  let publicKey='';
  fs.readFile(filePath, "utf8", (err, fileData) => {
    if (err) {
      console.error("Error reading chain.json:", err);
      return res.status(500).json({ error: "Error reading chain file" });
    }
    publicKey=fileData
  })
  // Spawn a child process to run the compiled C++ executable
  const cppProcess = spawn("./Algo/wallet/Transaction_main"); // Path to your compiled C++ executable

  // Send data to the C++ process via stdin
  cppProcess.stdin.write(sender + '\n');
  cppProcess.stdin.write(to + '\n');
  cppProcess.stdin.write(amount + '\n');
  cppProcess.stdin.end();

  // Capture the output from C++ process
  let signature = '';
  cppProcess.stdout.on('data', (data) => {
      signature += data.toString();
  });

  

  // Capture any errors from C++ process
  cppProcess.stderr.on('data', (data) => {
      console.error(`stderr: ${data}`);
  });

  cppProcess.on('close', (code) => {
      if (code === 0) {
        const data ={
          sender:sender,
          to:to,
          amount:amount,
          signature: signature,
          publicKey: publicKey
        }
          // Successfully signed the transaction
          res.json(data);
      } else {
          // Handle errors
          res.status(500).json({ error: 'Failed to sign transaction' });
      }
  });
};

function verifySignature(publicKey, signature, data) {
  const verify = crypto.createVerify('sha256');
  verify.update(data);
  verify.end();
  return verify.verify(publicKey, signature, 'hex');
}