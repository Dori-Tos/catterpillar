const express = require("express");
const mqtt = require("mqtt");
const path = require("path"); // Add this line
const app = express();
const server = require("http").Server(app);
const io = require("socket.io")(server);
require("dotenv").config();

// Serve static files from a 'public' directory
app.use(express.static(path.join(__dirname, "public")));

// Add a route for the root path
app.get("/", (req, res) => {
    res.sendFile(path.join(__dirname, "public", "index.html"));
});

// TTN MQTT Integration Configuration
const host = "eu1.cloud.thethings.network";
const port = 1883;
const username = "caterpillar@ttn";
const password = process.env.TTN_PASSWORD;

const clientId = `mqtt_${Math.random().toString(16).slice(3)}`;

const connectUrl = `mqtt://${host}:${port}`;

const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: username,
    password: password,
    reconnectPeriod: 1000,
});

client.on("connect", () => {
    console.log("Connected to TTN");
    client.subscribe([`v3/${username}/devices/+/up`], () => {
        console.log(`Subscribe to topic 'v3/${username}/devices/+/up'`);
    });
});

client.on("message", (topic, payload) => {
    console.log("Received Message:", topic, payload.toString());
    const message = JSON.parse(payload.toString());
    io.emit("ttn-event", message);
});

// Start the server
const port2 = process.env.PORT || 3000;
server.listen(port2, () => {
    console.log(`Server running on port ${port2}`);
});
