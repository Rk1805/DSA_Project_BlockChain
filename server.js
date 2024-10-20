const fs = require('fs');
const http = require('http');  // Use http instead of https
const express = require('express');
const WebSocket = require('ws');
const path = require('path');

// Set up an Express app
const app = express();
app.use(express.static(path.join(__dirname, 'public')));

const privateKey = fs.readFileSync('key.pem', 'utf8');
const certificate = fs.readFileSync('cert.pem', 'utf8');
const credentials = { key: privateKey, cert: certificate, passphrase: 'Rishi1112005' };

app.get('/', (req, res) => {
    res.send('Welcome to the server!');  // You can customize this message or serve an HTML file
  });
  
// Create an HTTP server
const httpServer = http.createServer(app);  // Use httpServer here

// Set up WebSocket server for signaling
const wss = new WebSocket.Server({ server: httpServer });  // Change to httpServer

// Store WebSocket connections
let peers = {};

wss.on('connection', (ws) => {
  ws.on('message', (message) => {
    const data = JSON.parse(message);
    const { type, payload, from, to } = data;

    switch (type) {
      case 'register':
        peers[from] = ws;
        console.log(`${from} has registered`);
        break;
      case 'offer':
      case 'answer':
      case 'candidate':
        if (peers[to]) {
          peers[to].send(JSON.stringify({ type, payload, from }));
        }
        break;
      case 'disconnect':
        delete peers[from];
        break;
    }
  });

  ws.on('close', () => {
    Object.keys(peers).forEach((peer) => {
      if (peers[peer] === ws) {
        delete peers[peer];
      }
    });
  });
});

// Start HTTP server
const PORT = 8080;  // Use port 8080 for HTTP
httpServer.listen(PORT, () => {
  console.log(`Server is listening on http://localhost:${PORT}`);
});
