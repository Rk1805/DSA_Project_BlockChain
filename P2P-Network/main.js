const peerIdInput = document.getElementById('peerId');
const connectBtn = document.getElementById('connectBtn');
const sendBtn = document.getElementById('sendBtn');
const messageInput = document.getElementById('message');
const dataChannelArea = document.getElementById('dataChannel');

// Create a WebSocket connection for signaling
const ws = new WebSocket('wss://localhost:8443');
let localPeerId = Math.random().toString(36).substring(2, 15);
let peerConnection;
let dataChannel;

// Register with signaling server
ws.onopen = () => {
  ws.send(JSON.stringify({
    type: 'register',
    from: localPeerId,
  }));
};

// Handle signaling messages
ws.onmessage = (message) => {
  const data = JSON.parse(message.data);
  const { type, payload, from } = data;

  switch (type) {
    case 'offer':
      handleOffer(payload, from);
      break;
    case 'answer':
      handleAnswer(payload);
      break;
    case 'candidate':
      handleCandidate(payload);
      break;
  }
};

// WebRTC configuration
const configuration = {
  iceServers: [
    { urls: 'stun:stun.l.google.com:19302' }
  ]
};

// Handle WebRTC offer
function handleOffer(offer, from) {
  createPeerConnection();
  peerConnection.setRemoteDescription(new RTCSessionDescription(offer));
  peerConnection.createAnswer()
    .then((answer) => {
      peerConnection.setLocalDescription(answer);
      ws.send(JSON.stringify({
        type: 'answer',
        payload: answer,
        from: localPeerId,
        to: from,
      }));
    });
}

// Handle WebRTC answer
function handleAnswer(answer) {
  peerConnection.setRemoteDescription(new RTCSessionDescription(answer));
}

// Handle ICE candidates
function handleCandidate(candidate) {
  peerConnection.addIceCandidate(new RTCIceCandidate(candidate));
}

// Create WebRTC peer connection
function createPeerConnection() {
  peerConnection = new RTCPeerConnection(configuration);

  peerConnection.ondatachannel = (event) => {
    dataChannel = event.channel;
    dataChannel.onmessage = (event) => {
      dataChannelArea.value += `Peer: ${event.data}\n`;
    };
  };

  peerConnection.onicecandidate = (event) => {
    if (event.candidate) {
      ws.send(JSON.stringify({
        type: 'candidate',
        payload: event.candidate,
        from: localPeerId,
        to: peerIdInput.value
      }));
    }
  };
}

// Initiate a connection to a peer
connectBtn.onclick = () => {
  createPeerConnection();
  dataChannel = peerConnection.createDataChannel('chat');
  dataChannel.onmessage = (event) => {
    dataChannelArea.value += `Peer: ${event.data}\n`;
  };

  peerConnection.createOffer()
    .then((offer) => {
      peerConnection.setLocalDescription(offer);
      ws.send(JSON.stringify({
        type: 'offer',
        payload: offer,
        from: localPeerId,
        to: peerIdInput.value
      }));
    });
};

// Send message via data channel
sendBtn.onclick = () => {
  const message = messageInput.value;
  dataChannelArea.value += `You: ${message}\n`;
  dataChannel.send(message);
  messageInput.value = '';
};
