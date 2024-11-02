import { exec } from "child_process";
import fetch from "node-fetch"
import { db } from "../config/firebase.js"; // Ensure this imports your Firestore instance
import { collection,addDoc,getDocs, query, where} from "firebase/firestore";

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
    const peersSnapshot = await getDocs(collection(db, "peers"));
    const peerIPs = peersSnapshot.docs.map((doc) => doc.data().ip);
    res.json({ peers: peerIPs });
  } catch (error) {
    console.error("Error fetching peers:", error);
    res.status(500).send("Failed to retrieve peer IP addresses");
  }
};

export const broadcast = async (req, res) => {
  const { peers, data } = req.body;

  const onlinePeers = [];
  const jsonData = JSON.stringify(data);  // Convert data to JSON string

  await Promise.all(
    peers.map((ip) => {
      return new Promise((resolve) => {
        try {
          const targetPort = 6001;

          const client = new net.Socket();
          client.connect(targetPort, ip, () => {
            console.log(`Connected to peer at ${ip}:${targetPort}`);
            client.write(jsonData);  // Send JSON data
          });

          client.on('data', (response) => {
            console.log(`Received response from peer ${ip}:`, response.toString());
            onlinePeers.push(ip);
            client.destroy();
            resolve();
          });

          client.on('close', () => {
            console.log(`Connection closed with peer ${ip}`);
          });

          client.on('error', (err) => {
            console.error(`Connection error with peer ${ip}:`, err.message);
            client.destroy();
            resolve();
          });
        } catch (error) {
          console.error(`Failed to connect to ${ip}:`, error.message);
          resolve();
        }
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
