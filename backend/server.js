import express from "express";
import cors from "cors";
import dotenv from "dotenv";
import bodyParser from "body-parser";
import net from "net";
import transactionRoutes from "./routes/transactionRoutes.js";
import { recieveTransaction } from "./controllers/transactionController.js";

dotenv.config();

const app = express();
app.use(express.json());
app.use(bodyParser.json());
app.use(cors());

// Initialize routes for HTTP API
app.use("/", transactionRoutes);

// HTTP server for the web application
const WEB_PORT = 5001;
app.listen(WEB_PORT, () => {
  console.log(`Web server is running on http://localhost:${WEB_PORT}`);
});

// TCP server for P2P network communication
const P2P_PORT = 6001;

const server = net.createServer((socket) => {
  console.log("New P2P connection established");

  socket.on("data", (data) => {
    const dataString = data.toString();
    console.log("Received raw data:", dataString);

    // Split by line breaks to parse HTTP-like structure
    const parts = dataString.split('\r\n\r\n'); // HTTP headers and body are separated by '\r\n\r\n'
    const jsonData = parts[1]; // Assuming JSON data is in the body

    try {
      const parsedData = JSON.parse(jsonData); // Parse JSON data
      console.log("Parsed JSON data:", parsedData);

      // Send acknowledgment back to the sender in JSON format
      const acknowledgmentMessage = JSON.stringify({ message: "Data received successfully" });
      socket.write(acknowledgmentMessage);
    } catch (err) {
      console.error("Error parsing JSON:", err.message);
      socket.write(JSON.stringify({ error: "Invalid JSON format" }));
    }
  });

  socket.on("close", () => {
    console.log("P2P connection closed");
  });

  socket.on("error", (err) => {
    console.error("Connection error:", err.message);
  });
});

// Start the TCP P2P server
server.listen(P2P_PORT, () => {
  console.log(`P2P server is listening on port ${P2P_PORT}`);
});
