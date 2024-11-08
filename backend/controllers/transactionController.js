import { exec } from "child_process";
import fetch from "node-fetch"
import { db } from "../config/firebase.js"; // Ensure this imports your Firestore instance
import { collection,addDoc,getDocs, query, where} from "firebase/firestore";
import net from "net";
import {spawn} from "child_process";
import fs from "fs";
import express from "express";

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
      return res.status(200).send({ id:userData.id });
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
  const { sender,amount,to,signature } = req.body;
  console.log("Received broadcast_data:", sender);
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
  const jsonData = {
    sender:sender,
    to:to,
    amount:amount,
    signature:signature,
  }; 

  await Promise.all(
    peerIPs.map((ip) => {
      return new Promise((resolve) => {
        const targetPort = 6001;
        const client = new net.Socket();
        const timeout = setTimeout(() => {
          console.error(`Timeout on peer ${ip}`);
          client.destroy(); // Close client if timeout occurs
          resolve();
        }, 2000); // 5-second timeout
  
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


export const recieveTransaction = (req, res) => {
  const { data } = req.body;

  res.send("successful");
};

export const signTransaction = (req, res) => {

  const { sender, to, amount } = req.body;

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
          // Successfully signed the transaction
          res.json({ signature });
      } else {
          // Handle errors
          res.status(500).json({ error: 'Failed to sign transaction' });
      }
  });
};
