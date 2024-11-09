import express from "express";
import cors from "cors";
import dotenv from "dotenv";
import bodyParser from "body-parser";
import net from "net";
import transactionRoutes from "./routes/transactionRoutes.js";
import historyRoutes from "./routes/historyRoutes.js";
import balanceRoutes from "./routes/balanceRoutes.js"

dotenv.config();

const app = express();
app.use(express.json());
app.use(bodyParser.json());
app.use(cors());

// Initialize routes for HTTP API
app.use("/", transactionRoutes);
app.use("/history",historyRoutes);
app.use("/balance",balanceRoutes);

// HTTP server for the web application
const WEB_PORT = 5001;
app.listen(WEB_PORT, () => {
  console.log(`Web server is running on http://localhost:${WEB_PORT}`);
});

// TCP server for P2P network communication
const P2P_PORT = 6001;

const server = net.createServer((socket) => {
  console.log("New P2P connection established");

  socket.on("data", async (data) => {
    const dataString = data.toString();
    console.log("Received raw data:", dataString);

    try {
      // Parse JSON data from the TCP connection
      const parsedData = JSON.parse(dataString);
      console.log("Parsed JSON data:", parsedData);

      // Send parsed data to the /storeData API endpoint
      const response = await fetch("http://localhost:5001/recieveTransaction", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ data: parsedData }),
      });

      const data = await response.json();
      console.log(data);
      if (response.ok) {
        console.log("Data sent to API successfully.");
        socket.write(JSON.stringify({ message: "Data received and stored successfully" }));
      } else {
        console.error("Error response from API:", response.statusText);
        socket.write(JSON.stringify({ error: "Failed to store data in API" }));
      }
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
