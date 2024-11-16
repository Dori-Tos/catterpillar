const express = require("express");
const app = express();
const server = require("http").Server(app);
const io = require("socket.io")(server);
const mqtt = require("mqtt");
const path = require("path");
require("dotenv").config();

const routes = require("./routes/routes");

// Serve static files from a 'public' directory
app.use(express.static(path.join(__dirname, "public")));

app.use("/", routes);

// TTN MQTT Integration Configuration
const TTN_HOST = "eu1.cloud.thethings.network";
const TTN_PORT = "1883";
const TTN_USERNAME = "caterpillar@ttn";
const TTN_PASSWORD = process.env.TTN_PASSWORD;

const clientId = `mqtt_${Math.random().toString(16).slice(3)}`;

const connectUrl = `mqtt://${TTN_HOST}:${TTN_PORT}`;

const client = mqtt.connect(connectUrl, {
    clientId,
    clean: true,
    connectTimeout: 4000,
    username: TTN_USERNAME,
    password: TTN_PASSWORD,
    reconnectPeriod: 1000,
});

let initialConnectionAttempted = false;
let connected = false;
client.on("connect", () => {
    initialConnectionAttempted = true;
    connected = true;
    console.log("Connected to TTN");
    client.subscribe([`v3/${TTN_USERNAME}/devices/+/up`], () => {
        console.log(`Subscribe to topic 'v3/${TTN_USERNAME}/devices/+/up'`);
    });
});

client.on("error", (error) => {
    if (!initialConnectionAttempted) {
        console.log(">> Failed to connect to TTN:\n", error);
        initialConnectionAttempted = true;
    } else if (connected) {
        console.log(">> Connection to TTN lost:\n", error);
        connected = false;
    }
});

client.on("message", (topic, payload) => {
    console.log("Received Message:", topic, payload.toString());
    const message = JSON.parse(payload.toString());
    io.emit("ttn-event", message);
});

// Start the server
const PORT = process.env.PORT || 3000;
server.listen(PORT, (error) => {
    if (!error) {
        console.log(`Server running on port ${PORT}`);
    } else {
        console.log(error);
    }
});
