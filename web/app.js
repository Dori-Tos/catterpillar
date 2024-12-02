const express = require("express");
const app = express();
const server = require("http").Server(app);
const io = require("socket.io")(server);
const mqtt = require("mqtt");
const path = require("path");
require("dotenv").config();

// Set EJS as the template engine
app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));
app.use(express.urlencoded({ extended: true }));
app.use(express.static(path.join(__dirname, "public"))); // Serve static files

// MQTT Configuration
const TTN_HOST = "eu1.cloud.thethings.network";
const TTN_PORT = "1883";
const TTN_USERNAME = "caterpillar@ttn";
const TTN_PASSWORD = process.env.TTN_PASSWORD;

const connectUrl = `mqtt://${TTN_HOST}:${TTN_PORT}`;
const client = mqtt.connect(connectUrl, {
    clientId: `mqtt_${Math.random().toString(16).slice(3)}`,
    username: TTN_USERNAME,
    password: TTN_PASSWORD,
    reconnectPeriod: 1000,
});

client.on("connect", () => {
    console.log("Connected to TTN");
    client.subscribe([`v3/${TTN_USERNAME}/devices/+/up`], () => {
        console.log("Subscribed to TTN topic");
    });
});

client.on("error", (error) => console.error("TTN MQTT error:", error));

client.on("message", (topic, message) => {
    console.log("Received TTN message:", topic, message.toString());
    const parsedMessage = JSON.parse(message.toString());

    const vehicleData = {
        id: parsedMessage.device_id,
        type: parsedMessage.type || "Unknown",
        lat: parsedMessage.location.latitude,
        lng: parsedMessage.location.longitude,
        lastUser: parsedMessage.lastUser || "Unknown",
        needsAssistance: parsedMessage.needsAssistance || false,
        history: parsedMessage.history || [],
    };

    io.emit("ttn-event", { vehicles: [vehicleData] });
});

// Route
app.get("/", (req, res) => {
    res.render("index");
});

// Start server
const PORT = process.env.PORT || 3000;
server.listen(PORT, () => console.log(`Server running on port ${PORT}`));
