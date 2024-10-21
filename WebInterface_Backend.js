const express = require('express');
const mqtt = require('mqtt');
const http = require('http');
const socketIO = require('socket.io');
const path = require('path');

// Set up express with socket.io
const app = express();
const server = http.Server(app);
const io = socketIO(server);

// Serve static files (like HTML, CSS, frontend JS)
app.use(express.static(path.join(__dirname, 'public')));

// Add your TTN credentials here
const appId = 'caterpillar'; 
const accessKey = 'NNSXS.34STHQSUPUGIOHQVJ77RSLR57GCVCSU25QNPQJI.EDVNYJQZMMFDN6GVBZ5QWZNMSMWFHCQX2HPYL6DWGDUJUTLYXGOA'; 

// Connect to TTN and subscribe to your device
const options = {
  username: appId,
  password: accessKey,
  clientId: `mqtt_${Math.random().toString(16).slice(3)}`
};

const client = mqtt.connect(`mqtts://${appId}.eu1.cloud.thethings.network`, options);

client.on('connect', () => {
  const deviceTopic = `v3/${appId}/devices/caterpillar-end-device/up`; 
  client.subscribe(deviceTopic, (err) => {
    if (!err) {
      console.log(`Subscribed to ${deviceTopic}`);
    }
  });
});

client.on('message', (topic, message) => {
  io.emit('event-name', message.toString());
});

server.listen(3000, () => {
  console.log('Server listening on port 3000');
});
